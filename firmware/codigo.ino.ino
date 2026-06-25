// --- LIBRERÍAS ---
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

// 1. ASIGNACIÓN DE PINES
const int pinMQ9 = A0;      
const int pinDSM501A = 5;   
const int pinLedRojo = 15;  
const int pinLedVerde = 13; 
const int pinLedAzul = 4;   

// 2. VARIABLES DEL SENSOR DE POLVO (DSM501A)
unsigned long duracionPulso;
unsigned long tiempoInicio;
unsigned long tiempoOcupacionBaja = 0; 
float proporcion = 0;                  
float concentracionPM25 = 0;           
// Ajustado a 15 segundos para la presentación (Límite mínimo de ThingSpeak)
const unsigned long TIEMPO_MUESTREO = 15000; 

// Parámetros de calibración DSM501A
float offsetPolvo = 0.0; // Cambiar si en aire limpio entrega valores > 0

// 3. VARIABLES DEL SENSOR DE GAS (MQ9)
int valorMQ9Crudo = 0; 
// Parámetros de calibración MQ9
float R0 = 10.0; 
float factorResistencia = 9.9; 

// 4. CREDENCIALES DE RED Y NUBE
const char* ssid = "ProyectoSCA";       
const char* password = "12345678";    
unsigned long myChannelNumber = 3416297;   // Reemplazar con ID de ThingSpeak
const char * myWriteAPIKey = "3K7351PSPW1ZC6VX"; // Reemplazar con Write API Key

WiFiClient clienteWiFi;

void setup() {
  Serial.begin(115200);
  delay(10);

  pinMode(pinDSM501A, INPUT);
  pinMode(pinLedRojo, OUTPUT);
  pinMode(pinLedVerde, OUTPUT);
  pinMode(pinLedAzul, OUTPUT);

  digitalWrite(pinLedRojo, LOW);
  digitalWrite(pinLedVerde, LOW);
  digitalWrite(pinLedAzul, LOW);

  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi conectado exitosamente.");

  ThingSpeak.begin(clienteWiFi);
  tiempoInicio = millis();
}

void loop() {
  // 1. LECTURA DEL SENSOR DE POLVO (DSM501A)
  duracionPulso = pulseIn(pinDSM501A, LOW);
  tiempoOcupacionBaja += duracionPulso;

  if ((millis() - tiempoInicio) >= TIEMPO_MUESTREO) {
    
    // Calentamiento reducido a 15 segundos para no demorar la presentación
    if (millis() < 15000) {
      Serial.println("Calentando sensor óptico... (descartando datos)");
      tiempoOcupacionBaja = 0;
      tiempoInicio = millis();
      return; 
    }

    // 2. CÁLCULO DE MATERIAL PARTICULADO (ug/m3)
    proporcion = tiempoOcupacionBaja / (TIEMPO_MUESTREO * 10.0); 
    concentracionPM25 = 1.1 * pow(proporcion, 3) - 3.8 * pow(proporcion, 2) + 520 * proporcion + 0.62;
    
    concentracionPM25 = concentracionPM25 - offsetPolvo;
    if (concentracionPM25 < 0) concentracionPM25 = 0;

    // 3. LECTURA Y CÁLCULO DE GAS CO (ppm)
    valorMQ9Crudo = analogRead(pinMQ9); 
    float voltajeSensor = valorMQ9Crudo * (3.3 / 1023.0);
    float Rs = ((5.0 * 10.0) / voltajeSensor) - 10.0; 
    float ratio = Rs / R0;
    float ppmCO = pow(10, ((log10(ratio) - 0.99) / -0.45)); 

    Serial.print("PM2.5 (ug/m3): ");
    Serial.println(concentracionPM25);
    Serial.print("CO (ppm): ");
    Serial.println(ppmCO);

    // 4. LÓGICA DE ALERTAS (Normativa DS 594)
    if (ppmCO >= 40 || concentracionPM25 >= 150) {
      digitalWrite(pinLedRojo, HIGH);
      digitalWrite(pinLedVerde, LOW);
      digitalWrite(pinLedAzul, LOW);
    } 
    else if ((ppmCO >= 21 && ppmCO < 40) || (concentracionPM25 >= 51 && concentracionPM25 < 150)) {
      digitalWrite(pinLedRojo, HIGH);
      digitalWrite(pinLedVerde, HIGH);
      digitalWrite(pinLedAzul, LOW);
    } 
    else {
      digitalWrite(pinLedRojo, LOW);
      digitalWrite(pinLedVerde, HIGH);
      digitalWrite(pinLedAzul, LOW);
    }

    // 5. TRANSMISIÓN A LA NUBE (ThingSpeak)
    ThingSpeak.setField(1, concentracionPM25);
    ThingSpeak.setField(2, ppmCO);

    int codigoRespuesta = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    
    if (codigoRespuesta == 200) {
      Serial.println("Datos enviados a ThingSpeak.");
    } else {
      Serial.print("Error de transmisión HTTP: ");
      Serial.println(codigoRespuesta);
    }

    // 6. REINICIO DEL CICLO
    tiempoOcupacionBaja = 0;
    tiempoInicio = millis();
  }
}