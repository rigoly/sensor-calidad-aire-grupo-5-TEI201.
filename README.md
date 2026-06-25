# Detector de Calidad del Aire en Mineras Cerradas

**Descripción:** Sistema IoT diseñado para medir continuamente los niveles de monóxido de carbono (CO) y material particulado (PM2.5) en entornos mineros, alertando sobre condiciones de riesgo para los trabajadores.

**Integrantes:** Martin Guerrero, Arturo Luks, Lukas Vallejos.

**Problema que resuelve:** Monitorizar la acumulación de gases y polvo en minas cerradas para prevenir intoxicaciones y problemas respiratorios.

**Componentes necesarios:**
- Microcontrolador ESP8266 (NodeMCU 1.0)
- Sensor de gas MQ9 (CO)
- Sensor de polvo óptico DSM501A (PM2.5)
- Módulo LED RGB CNT1
- Protoboard y jumpers

**Instrucciones de uso:**
1. Instalar la librería `ThingSpeak` en Arduino IDE.
2. Configurar un punto de acceso móvil en 2.4 GHz e ingresar las credenciales WiFi en el código.
3. Compilar y cargar el firmware en la placa ESP8266.
4. **Dashboard:** 3416297
