# ESP32 AP+STA con NAT e Internet Compartido

Proyecto avanzado de ESP32 que implementa un **router WiFi completo** mediante la combinación de modo Access Point (AP) y Station (STA) con traducción de direcciones de red (NAT), permitiendo compartir Internet desde un router existente hacia dispositivos conectados al ESP32.

---

##  Introducción al Proyecto

Este proyecto transforma un ESP32 en un **repetidor WiFi inteligente** que:

1. **Se conecta como cliente (STA)** a un router WiFi existente con Internet
2. **Crea su propio punto de acceso (AP)** al que otros dispositivos pueden conectarse
3. **Comparte la conexión a Internet** mediante NAT (Network Address Translation)
4. **Configura automáticamente DNS** para que aplicaciones y navegadores funcionen correctamente

### ¿Qué es el Modo AP+STA?

El ESP32 funciona simultáneamente en dos modos:
- **Station (STA)**: Actúa como cliente WiFi conectándose al router de casa "FAMILIA-GP"
- **Access Point (AP)**: Crea una red WiFi propia llamada "Jueguru" donde otros dispositivos se conectan

### ¿Qué es NAT y por qué es necesario?

**NAT (Network Address Translation)** es la tecnología que permite que múltiples dispositivos compartan una única conexión a Internet:

- Los dispositivos conectados al AP tienen IPs privadas (192.168.4.x)
- El ESP32 traduce estas IPs a su propia IP pública (192.168.1.x)
- El router ve solo al ESP32, no a los dispositivos individuales
- **Sin NAT**, los dispositivos podrían conectarse al AP pero no tendrían acceso a Internet

---

## Arquitectura del Sistema

```
Internet
   |
   ↓
[Router WiFi: FAMILIA-GP]
   |
   ↓ (Modo STA)
[ESP32] ← IP: 192.168.1.135
   |
   ↓ (Modo AP + NAT)
[Punto de Acceso: Jueguru]
   |
   ↓
[Dispositivos Conectados]
- Móvil: 192.168.4.2
- Laptop: 192.168.4.3
- Tablet: 192.168.4.4
```

---

##  Características Principales

### 1. Modo Dual WiFi (AP+STA)
- **STA**: Conexión automática con reintentos al router principal
- **AP**: Red propia con contraseña WPA2-PSK segura
- Soporte para hasta 4 dispositivos simultáneos

### 2. NAT Completo (lwIP NAPT)
- Traducción de direcciones de red en tiempo real
- Enrutamiento IP automático entre interfaces
- Activación dinámica al obtener conexión a Internet

### 3. Configuración DNS Inteligente
- **DNS primario**: Google DNS (8.8.8.8)
- **DNS secundario**: Cloudflare DNS (1.1.1.1)
- **DNS dinámico**: Usa el gateway del router como alternativa
- Entrega automática de DNS vía DHCP a clientes

### 4. Gestión Robusta de Eventos
- Reconexión automática en caso de pérdida de señal
- Logs detallados para depuración
- Activación sincronizada de NAT solo cuando hay Internet

---

##  Componentes del Proyecto

### Estructura de Archivos

```
proyecto/
├── main/
│   ├── main.c           # Punto de entrada y orquestación
│   ├── wifi_ap.c        # Configuración del Access Point
│   ├── wifi_ap.h        # Header del AP
│   ├── wifi_sta.c       # Configuración del Station
│   └── wifi_sta.h       # Header del STA
├── CMakeLists.txt
└── sdkconfig            # Configuración con NAT habilitado
```

### Descripción Detallada de Archivos

#### `main.c` - Controlador Principal
**Responsabilidades:**
- Inicialización del sistema (NVS, TCP/IP, Event Loop)
- Registro del manejador de eventos IP
- Activación de NAT cuando el STA obtiene IP
- Configuración dinámica de DNS para el AP

**Flujo de Ejecución:**
1. Inicializa almacenamiento NVS (necesario para WiFi)
2. Crea el stack TCP/IP y el bucle de eventos
3. Registra un manejador para el evento `IP_EVENT_STA_GOT_IP`
4. Inicia WiFi en modo dual
5. Espera conexión al router para activar NAT

**Código Clave - Manejador de Eventos:**
```c
static void event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data)
{
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        // 1. Obtener IP del router
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        
        // 2. Activar NAT en la interfaz AP
        esp_netif_t *ap_netif = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
        esp_netif_ip_info_t ap_ip_info;
        esp_netif_get_ip_info(ap_netif, &ap_ip_info);
        ip_napt_enable(ap_ip_info.ip.addr, 1);  // ← Magia del NAT
        
        // 3. Configurar DNS para que los clientes puedan navegar
        esp_netif_dns_info_t dns;
        dns.ip.u_addr.ip4.addr = event->ip_info.gw.addr;  // Gateway del router
        esp_netif_set_dns_info(ap_netif, ESP_NETIF_DNS_MAIN, &dns);
    }
}
```

#### `wifi_sta.c` - Cliente WiFi (Station)
**Responsabilidades:**
- Conexión al router principal
- Reconexión automática en caso de desconexión
- Manejo de eventos de conexión/desconexión
- Logs de información de red (IP, máscara, gateway)

**Configuración:**
```c
#define CASA_SSID      "*******"      // ← Cambiar por tu WiFi
#define CASA_PASSWORD  "*******"      // ← Cambiar por tu contraseña
```

**Eventos Manejados:**
- `WIFI_EVENT_STA_START`: Intenta conectar al router
- `WIFI_EVENT_STA_DISCONNECTED`: Reintenta conexión automáticamente
- `IP_EVENT_STA_GOT_IP`: Registra información de red obtenida

#### `wifi_ap.c` - Punto de Acceso (Access Point)
**Responsabilidades:**
- Crear red WiFi propia
- Configurar servidor DHCP con DNS
- Asignar IPs automáticamente a clientes (192.168.4.2 - 192.168.4.254)
- Entregar servidores DNS a dispositivos conectados

**Configuración Crítica:**
```c
#define Nombre "Jueguru"              // SSID del AP
#define Contraseña "Jueguru123"       // Contraseña (mínimo 8 caracteres)
#define Max_Conexiones 4              // Dispositivos simultáneos
```

**Proceso de Configuración DNS:**
1. **Detener DHCP** temporalmente
2. **Configurar IP estática** del AP (192.168.4.1)
3. **Habilitar opción DNS** en el servidor DHCP
4. **Configurar DNS primario** (8.8.8.8) y secundario (1.1.1.1)
5. **Reiniciar DHCP** con la nueva configuración

Esto asegura que cuando un dispositivo se conecta al AP, recibe:
- IP automática (ej: 192.168.4.2)
- Gateway: 192.168.4.1 (el ESP32)
- DNS: 8.8.8.8 y 1.1.1.1 (para resolver dominios)

---

##  Herramientas y APIs de ESP-IDF Utilizadas

### WiFi (esp_wifi.h)
- `esp_wifi_init()` - Inicializa el driver WiFi
- `esp_wifi_set_mode(WIFI_MODE_APSTA)` - Activa modo dual AP+STA
- `esp_wifi_set_config()` - Configura parámetros de STA y AP
- `esp_wifi_start()` - Inicia las interfaces WiFi
- `esp_wifi_connect()` - Conecta al router en modo STA

### Networking (esp_netif.h)
- `esp_netif_init()` - Inicializa el stack TCP/IP (lwIP)
- `esp_netif_create_default_wifi_ap()` - Crea interfaz de red para AP
- `esp_netif_create_default_wifi_sta()` - Crea interfaz de red para STA
- `esp_netif_dhcps_stop/start()` - Controla servidor DHCP
- `esp_netif_set_dns_info()` - Configura servidores DNS
- `esp_netif_get_ip_info()` - Obtiene información de red (IP, máscara, gateway)

### NAT (lwip/lwip_napt.h)
- `ip_napt_enable()` - Activa traducción de direcciones de red
  - Parámetro 1: IP del AP (192.168.4.1)
  - Parámetro 2: 1 para habilitar, 0 para deshabilitar

### Eventos (esp_event.h)
- `esp_event_loop_create_default()` - Crea el bucle de eventos del sistema
- `esp_event_handler_register()` - Registra callbacks para eventos específicos
- Eventos clave:
  - `WIFI_EVENT_STA_START` - STA iniciado
  - `WIFI_EVENT_STA_DISCONNECTED` - Conexión perdida
  - `IP_EVENT_STA_GOT_IP` - IP obtenida del router (trigger para NAT)

### DHCP (dhcpserver/dhcpserver.h)
- `esp_netif_dhcps_option()` - Configura opciones del servidor DHCP
- `OFFER_DNS` - Habilita entrega de servidores DNS a clientes

### Utilidades
- `ESP_ERROR_CHECK()` - Verifica y maneja errores
- `ESP_LOGI/LOGW/LOGE()` - Logs con diferentes niveles de severidad
- `nvs_flash_init()` - Inicializa almacenamiento no volátil

---

##  Configuración y Compilación

### Requisitos Previos
- ESP-IDF v5.5.1 o superior
- Visual Studio Code con extensión ESP-IDF
- Placa ESP32 (cualquier modelo con WiFi)

### Paso 1: Habilitar NAT en ESP-IDF

**CRÍTICO**: Por defecto, ESP-IDF NO incluye soporte NAT. Debe habilitarse manualmente.

#### Método A: Usando menuconfig (Recomendado)
```bash
# En VS Code:
Ctrl + Shift + P → "ESP-IDF: SDK Configuration editor (menuconfig)"

# Navegar a:
Component config → LWIP → ☑ Enable IP forwarding
Component config → LWIP → ☑ Enable NAT (IP_NAPT)

# Guardar: S → Enter → Q
```

#### Método B: Editar sdkconfig manualmente
```ini
# Agregar estas líneas al archivo sdkconfig:
CONFIG_LWIP_IP_FORWARD=y
CONFIG_LWIP_IPV4_NAPT=y
```

### Paso 2: Configurar Credenciales WiFi

Editar `wifi_sta.c`:
```c
#define CASA_SSID      "TU_WIFI"        // ← Cambiar
#define CASA_PASSWORD  "TU_CONTRASEÑA"  // ← Cambiar
```

### Paso 3: Compilar y Flashear
```bash
# En VS Code:
Ctrl + Shift + P → "ESP-IDF: Build your project"
Ctrl + Shift + P → "ESP-IDF: Flash your project"
Ctrl + Shift + P → "ESP-IDF: Monitor your device"
```

---

##  Salida Esperada del Monitor Serial

### Secuencia de Inicio Correcta
```
I (497) MAIN: Iniciando ESP32 en modo AP+STA con Internet compartido
I (617) WIFI STA: Cliente STA configurado → SSID: FAMILIA-GP
I (747) WIFI AP: DHCP Server iniciado con DNS: 8.8.8.8 y 1.1.1.1
I (757) WIFI AP: Punto de acceso AP iniciado. SSID:Jueguru, Contraseña:Jueguru123
I (767) MAIN: Esperando conexión al router para activar NAT...
I (867) wifi: connected with FAMILIA-GP, aid = 18, channel 8, BW20
I (4187) MAIN: CONECTADO AL ROUTER
I (4187) MAIN: IP del ESP32: 192.168.1.135
I (4187) MAIN: ✓ NAT ACTIVADO - IP: 192.168.4.1
I (4197) MAIN: ✓ DNS configurado
I (4197) WIFI STA: CONECTADO AL ROUTER
I (4197) WIFI STA: IP: 192.168.1.135
I (4197) WIFI STA: Gateway: 192.168.1.1
```

### Indicadores de Éxito
"NAT ACTIVADO" aparece en los logs  
"DHCP Server iniciado con DNS" aparece antes del arranque del AP  
"CONECTADO AL ROUTER" con IP válida  

---

##  Problemas Comunes y Soluciones

### Problema 1: Error de Compilación - `lwip/lwip_napt.h: No such file or directory`

**Causa**: NAT no está habilitado en la configuración de ESP-IDF.

**Solución**:
```bash
# Opción 1: menuconfig
idf.py menuconfig
# Component config → LWIP → Habilitar IP_FORWARD y IPV4_NAPT

# Opción 2: Editar sdkconfig
CONFIG_LWIP_IP_FORWARD=y
CONFIG_LWIP_IPV4_NAPT=y

# Limpiar y recompilar:
Ctrl + Shift + P → "ESP-IDF: Full Clean"
Ctrl + Shift + P → "ESP-IDF: Build your project"
```

### Problema 2: `ESP_ERR_NOT_SUPPORTED (0x106)` al activar NAT

**Causa**: La función `esp_netif_napt_enable()` no existe. NAT debe activarse con `ip_napt_enable()` de lwIP.

**Solución**: Usar la API correcta de lwIP:
```c
// ❌ INCORRECTO:
esp_netif_napt_enable(esp_netif_get_handle_from_ifkey("WIFI_AP_DEF"));

// ✅ CORRECTO:
esp_netif_ip_info_t ap_ip_info;
esp_netif_get_ip_info(ap_netif, &ap_ip_info);
ip_napt_enable(ap_ip_info.ip.addr, 1);
```

### Problema 3: Apps de META funcionan pero Chrome/Spotify no

**Causa**: DNS no se está entregando correctamente a los dispositivos conectados.

**Síntomas**:
- WhatsApp, Instagram, Facebook funcionan (usan IPs cacheadas)
- Navegadores web y búsquedas no funcionan (necesitan DNS)

**Solución**: Configurar DHCP con DNS (implementado en `wifi_ap.c`):
```c
// 1. Detener DHCP
esp_netif_dhcps_stop(ap_netif);

// 2. Configurar opción DNS
dhcps_offer_t dhcps_dns_value = OFFER_DNS;
esp_netif_dhcps_option(ap_netif, ESP_NETIF_OP_SET, ESP_NETIF_DOMAIN_NAME_SERVER, 
                      &dhcps_dns_value, sizeof(dhcps_dns_value));

// 3. Configurar servidores DNS
esp_netif_dns_info_t dns_info;
dns_info.ip.u_addr.ip4.addr = ESP_IP4TOADDR(8, 8, 8, 8);  // Google DNS
esp_netif_set_dns_info(ap_netif, ESP_NETIF_DNS_MAIN, &dns_info);

// 4. Reiniciar DHCP
esp_netif_dhcps_start(ap_netif);
```

**Verificación en el dispositivo móvil:**
- Android: Ajustes → WiFi → Jueguru → Detalles avanzados → DNS debe ser `8.8.8.8`
- iOS: Ajustes → WiFi → (i) → DNS debe mostrar `8.8.8.8, 1.1.1.1`

**Importante**: Después de actualizar el código, **"olvidar" la red WiFi** en el móvil y reconectar para obtener la nueva configuración DHCP.

### Problema 4: Dispositivos se conectan pero no tienen Internet

**Causa**: NAT no se activó porque el ESP32 no obtuvo IP del router.

**Diagnóstico**:
```bash
# En el monitor serial, verificar:
grep "NAT ACTIVADO" output.log  # Debe aparecer
grep "CONECTADO AL ROUTER" output.log  # Debe aparecer con IP válida
```

**Solución**:
1. Verificar credenciales en `wifi_sta.c` (SSID y contraseña correctos)
2. Verificar que el router permita nuevas conexiones
3. Revisar la potencia de la señal WiFi

---

## 🔍 Depuración Avanzada

### Verificar Estado de NAT
```c
// Agregar en event_handler después de activar NAT:
ESP_LOGI(TAG, "Estado NAT verificado. Tabla NAPT inicializada.");
```

### Logs de DNS
```c
// Verificar DNS configurado:
esp_netif_dns_info_t dns_check;
esp_netif_get_dns_info(ap_netif, ESP_NETIF_DNS_MAIN, &dns_check);
ESP_LOGI(TAG, "DNS entregado a clientes: " IPSTR, IP2STR(&dns_check.ip.u_addr.ip4));
```

### Monitoreo de Conexiones
```c
// En wifi_ap.c, agregar manejador de eventos:
static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                               int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "Cliente conectado - MAC: " MACSTR, MAC2STR(event->mac));
    }
}
```

---

##  Mejoras Futuras

### Funcionalidades Adicionales
- **Portal Cautivo**: Página de configuración web para cambiar SSID/contraseña sin recompilar
- **Monitoreo de Ancho de Banda**: Limitar velocidad por dispositivo
- **Lista Blanca/Negra**: Control de acceso por dirección MAC
- **Servidor Web**: Panel de control con estadísticas en tiempo real
- **MQTT Bridge**: Reenvío de mensajes entre redes
- **VPN**: Túnel seguro a través del ESP32

### Optimizaciones de Rendimiento
- Ajustar tamaño de la tabla NAT según número de dispositivos
- Implementar QoS (Quality of Service) para priorizar tráfico
- Caché DNS local para reducir latencia

---

##  Referencias Técnicas

### Documentación Oficial
- [ESP-IDF Programming Guide](https://docs.espressif.com/projects/esp-idf/en/latest/)
- [ESP32 WiFi API Reference](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/network/esp_wifi.html)
- [lwIP Documentation](https://www.nongnu.org/lwip/)

### Conceptos Clave
- **NAT/NAPT**: Network Address (Port) Translation
- **DHCP**: Dynamic Host Configuration Protocol
- **DNS**: Domain Name System
- **AP+STA**: Modo simultáneo Access Point + Station

---

##  Licencia y Créditos

Proyecto desarrollado con ESP-IDF v5.5.1 para fines educativos.

**Nota de Seguridad**: Este proyecto es para uso doméstico y educativo. Para aplicaciones de producción, considera:
- Implementar autenticación WPA3
- Agregar firewall de aplicación
- Configurar límites de tasa (rate limiting)
- Logs de auditoría de seguridad

---

##  Conclusión

Este proyecto demuestra la capacidad del ESP32 para funcionar como un router WiFi completo con características avanzadas como NAT y gestión de DNS. A través de la implementación, se han abordado desafíos técnicos reales como:

1. **Integración de NAT en ESP-IDF** (que no está habilitado por defecto)
2. **Configuración correcta de DNS vía DHCP** (necesario para navegación web)
3. **Sincronización de eventos** entre interfaces STA y AP
4. **Manejo robusto de errores** y reconexiones automáticas

El resultado es un sistema funcional que puede extender el alcance de una red WiFi existente o crear redes aisladas con acceso controlado a Internet, útil para proyectos de IoT, domótica, o simplemente como herramienta de aprendizaje sobre redes y sistemas embebidos.
