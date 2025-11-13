# Ejemplo_1: ESP-IDF LED Blink with FreeRTOS

Pequeño proyecto de ejemplo para ESP-IDF que parpadea un LED e imprime "Hola Mundo" en la consola serial.

##  Requisitos

- ESP-IDF instalado y configurado en PATH
- Python 3.7+
- Herramientas de compilación (gcc, make)
- Puerto COM disponible para la placa ESP32

##  Inicio rápido

### Configurar opciones de compilación:
```powershell
idf.py menuconfig
```

### Compilar el proyecto:
```powershell
idf.py build
```

### Grabar y abrir monitor serial (reemplaza COM3 con tu puerto):
```powershell
idf.py -p COM3 flash monitor
```

### Limpiar artefactos de compilación:
```powershell
idf.py fullclean
```
.

