# ESP32 Web Server con Autenticación y Control GPIO

**Proyecto desarrollado por:** Sergio Andrés Pilonieta Garzón  
**Fecha:** Diciembre 2025  
**Versión ESP-IDF:** v5.5.1

Servidor web embebido en ESP32 que permite controlar pines GPIO de forma remota mediante una interfaz web moderna, con autenticación HTTP Basic y soporte para modo Access Point (AP) + Station (STA) simultáneo.

## INTRODUCCION:

✅ WiFi Dual: AP (red propia) + STA (conexión a router)
✅ Servidor HTTP: Control remoto desde navegador
✅ Autenticación: HTTP Basic para seguridad
✅ Control GPIO: LED en tiempo real
✅ Interfaz moderna: Diseño responsive con gradientes CSS
✅ Acceso global: Compatible con ngrok

## Características Principales

### 1. Modo Dual WiFi (AP+STA)
- **STA**: Conexión automática con reintentos al router principal
- **AP**: Red propia con contraseña WPA2-PSK segura
- Gestión robusta de eventos de conexión/desconexión
- Logs detallados para depuración

### 2. Servidor Web HTTP
- Servidor HTTP ligero basado en `esp_http_server`
- Sistema de rutas extensible
- Archivos HTML embebidos (no requiere sistema de archivos)
- Puerto configurable (por defecto: 80)

### 3. Autenticación HTTP Basic
- Protección mediante usuario y contraseña
- Decodificación Base64 con mbedTLS
- Respuestas 401 Unauthorized personalizadas
- Validación en cada petición HTTP

### 4. Control de GPIO
- Abstracción de hardware para fácil extensión
- Control del LED integrado (GPIO2)
- Estados bien definidos (LED_ON / LED_OFF)
- API limpia para agregar más pines

### 5. Interfaz Web Moderna
- Diseño responsive (funciona en móviles, tablets, PCs)
- Gradientes y animaciones CSS
- Control en tiempo real con fetch API
- Feedback visual del estado del LED

---

## Arquitectura del Sistema

```
┌─────────────────────────────────────────────────────┐
│                    INTERNET                         │
└─────────────────┬───────────────────────────────────┘
                  │
                  ↓
        ┌─────────────────────┐
        │  Router WiFi        │
        │  192.168.1.1        │
        └──────────┬──────────┘
                   │
                   ↓ (Modo STA)
        ┌─────────────────────┐
        │      ESP32          │
        │  IP STA: 192.168.1.x│ ← Asignada por DHCP del router
        │  IP AP:  192.168.4.1│ ← IP estática del AP
        │                     │
        │  ┌──────────────┐   │
        │  │ Web Server   │   │
        │  │ Puerto 80    │   │
        │  └──────────────┘   │
        │  ┌──────────────┐   │
        │  │ LED GPIO2    │   │
        │  └──────────────┘   │
        └──────────┬──────────┘
                   │
                   ↓ (Modo AP)
        ┌─────────────────────┐
        │  Punto de Acceso    │
        │  SSID: Jueguru      │
        │  Pass: 12345678     │
        └──────────┬──────────┘
                   │
        ┌──────────┴──────────┐
        │                     │
    ┌───▼────┐          ┌────▼───┐
    │ Móvil  │          │ Laptop │
    │192.168 │          │192.168 │
    │  .4.2  │          │  .4.3  │
    └────────┘          └────────┘
```

## Estructura del Proyecto
```
07_Servidor_Http/
├── main/
│   ├── main.c                      # Punto de entrada principal
│   ├── configuracion.h             # Configuración centralizada
│   ├── index.html                  # Interfaz web embebida
│   │
│   ├── Autenticacion/
│   │   ├── auth.h                  # API de autenticación
│   │   └── auth.c                  # Implementación HTTP Basic
│   │
│   ├── Control_pines/
│   │   ├── gpio_control.h          # API de control GPIO
│   │   └── gpio_control.c          # Implementación de pines
│   │
│   ├── Configuracion_wifi/
│   │   ├── wifi_manager.h          # API de gestión WiFi
│   │   └── wifi_manager.c          # Modo AP+STA
│   │
│   ├── Server_http/
│   │   ├── web_server.h            # API del servidor web
│   │   └── web_server.c            # Handlers y rutas HTTP
│   │
│   └── CMakeLists.txt              # Configuración de compilación
│
├── CMakeLists.txt                  # CMake del proyecto raíz
├── sdkconfig                       # Configuración ESP-IDF
└── README.md                       # Este archivo
```

---
## Inicio
1. Configurar Credenciales

Editar main/configuracion.h:
```c
// WiFi de tu router
#define WIFI_SSID_STA "TU_WIFI"          // ← CAMBIAR
#define WIFI_PASS_STA "TU_CONTRASEÑA"    // ← CAMBIAR

// Access Point del ESP32
#define WIFI_SSID_AP "Jueguru"
#define WIFI_PASS_AP "12345678"

// Autenticación HTTP
#define AUTH_USERNAME "YoSoyJueguru"     // ← CAMBIAR
#define AUTH_PASSWORD "Jueguru"    
```

## Acceder al Servidor
1) Opción A - Modo AP (Directo):

    Conectar al WiFi "Jueguru" (pass: 12345678)
    Abrir: http://192.168.4.1

2) Opción B - Modo STA (Router):

    Ver IP en logs: I (5187) WIFI: IP asignada: 192.168.1.X
    Abrir: http://192.168.1.X

## Mejoras Futuras

- API REST JSON para integración con apps
- WebSocket para actualizaciones en tiempo real
- OTA Updates para actualizar sin cable
- Portal de configuración web
- HTTPS/SSL para cifrado
- Control de múltiples GPIOs
- Dashboard con gráficas (Chart.js)
- Sistema de usuarios con base de datos


---

##  APIs ESP-IDF Utilizadas

| Componente | Funciones Principales |
|------------|----------------------|
| **WiFi** | `esp_wifi_init()`, `esp_wifi_set_mode()`, `esp_wifi_connect()` |
| **HTTP Server** | `httpd_start()`, `httpd_register_uri_handler()` |
| **GPIO** | `gpio_set_direction()`, `gpio_set_level()` |
| **Eventos** | `esp_event_handler_instance_register()` |
| **NVS** | `nvs_flash_init()` |
| **mbedTLS** | `mbedtls_base64_decode()` |

---

---

## 📖 Referencias

- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/)
- [ESP32 HTTP Server](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/protocols/esp_http_server.html)
- [ESP32 WiFi API](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_wifi.html)

---

