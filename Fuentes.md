# Declaración de Fuentes e Integridad Académica

## 1. Librerías y dependencias
| Librería | Versión | Uso en el proyecto | Fuente |
| :--- | :--- | :--- | :--- |
| ESP8266WiFi | Integrada | Conexión de la placa a la red WiFi móvil (2.4 GHz). | https://github.com/esp8266/Arduino |
| ThingSpeak | 2.0.0 | Envío de variables de PM2.5 y CO a la base de datos en la nube. | https://github.com/mathworks/thingspeak-arduino |

## 2. Código externo adaptado
### Fórmulas de conversión para sensores MQ9 y DSM501A
- **Fuente:** Datasheets oficiales de los fabricantes.
- **Adaptación:** Se implementó la ecuación logarítmica base para despejar las partes por millón (ppm) del sensor MQ9. Para el DSM501A, se utilizó la ecuación polinómica recomendada para calcular los microgramos por metro cúbico basándose en la proporción de tiempo en estado LOW (`pulseIn`).

## 3. Uso de Inteligencia Artificial
### Estructuración del firmware, lógica de umbrales y conexión WiFi
- **Herramienta:** Gemini (junio 2026).
- **Prompt utilizado:** Interacciones solicitando estructurar el código para ESP8266 leyendo un MQ9 y DSM501A, establecer alertas con un LED RGB según normativas de seguridad, y transmitir los datos a ThingSpeak.
- **Adaptación:** Se ajustó el tiempo de muestreo a 15 segundos para cumplir con el límite de peticiones de ThingSpeak. Se incluyó un bloque de descarte de datos iniciales para la estabilización térmica del sensor óptico y se implementó un offset manual para el material particulado. 
- **Comprensión:** El equipo comprende que el código utiliza `pulseIn` de manera semi-bloqueante, utiliza `analogRead` para convertir voltajes a concentraciones, y evalúa estas variables mediante compuertas lógicas (`||` y `&&`) para activar el LED RGB.
