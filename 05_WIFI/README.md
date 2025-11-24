# Configuración WiFi AP con LEDs
Proyecto en ESP32 que configura un punto de acceso WiFi (AP) y actualiza LEDs en función del número de dispositivos conectados, utilizando FreeRTOS para tareas en núcleos específicos.


## Introducción Breve al WiFi AP
En este proyecto para ESP32, se configura un Punto de Acceso WiFi (AP) que permite a otros dispositivos (como celulares o laptops) conectarse a él, formando una red local sin necesidad de un router externo:
    - **Configuración Principal**: El AP se llama "Jueguru" con contraseña "Jueguru123", opera en el canal 4, soporta hasta 4 conexiones simultáneas y usa autenticación WPA/WPA2 para seguridad.
    - **Funcionamiento**: Al iniciarse (en la tarea wifi_init_ap en el núcleo 1), registra eventos de conexión/desconexión. Cada conexión incrementa un contador global (conexiones), y cada desconexión lo decrementa. Esto actualiza LEDs: uno se enciende con ≥1 conexión (GPIO 2) y otro con ≥2 (GPIO 4).

Esto se integra con inicialización de LEDs en el núcleo 0, todo orquestado desde main.c.
## Beneficios

    - **Facilidad de Uso en IoT**: Ideal para crear redes locales privadas, como en proyectos de domótica o monitoreo, sin depender de infraestructura externa.
    
    - **Eficiencia con FreeRTOS**: Permite multitarea en núcleos duales del ESP32, optimizando el rendimiento y separando lógica de WiFi y LEDs.
    - **Visualización Intuitiva** : Los LEDs proporcionan feedback visual inmediato sobre el estado de conexiones, útil para depuración y demostraciones.
    -Seguridad Básica: Usa autenticación WPA/WPA2 para proteger la red, y logs en consola para monitoreo en tiempo real.
    -Escalabilidad: Fácil de expandir para más LEDs, conexiones o integraciones (ej. servidores web en el AP).

SECUENCIA DE CÓDIGOEl proyecto contiene un archivo principal donde se inicializan los componentes:main.cTenemos varios archivos auxiliares como:configuracion_wifi.c
luces.cEn estos se implementa la lógica para configurar el WiFi AP, manejar eventos de conexión/desconexión, inicializar LEDs y actualizar su estado basado en el número de conexiones. Se utiliza una variable global conexiones para rastrear el número de estaciones conectadas y comunicarlo entre módulos.Se crean tareas que se ejecutan en núcleos específicos, mostrando un ejemplo:xTaskCreatePinnedToCore(
    wifi_init_ap,         // Función de la tarea
    "wifi_init_ap",       // Nombre de la tarea
    8192,                 // Tamaño de stack
    NULL,                 // Parámetro de la tarea
    5,                    // Prioridad de la tarea
    NULL,                 // Puntero a la tarea creada
    1);                   // Core 1Otro ejemplo para inicializar LEDs:xTaskCreatePinnedToCore(
    inicia_leds,          // Función de la tarea
    "inicia_leds",        // Nombre de la tarea
    8192,                 // Tamaño de stack
    NULL,                 // Parámetro de la tarea
    5,                    // Prioridad de la tarea
    NULL,                 // Puntero a la tarea creada
    0);                   // Core 0Se utilizan funciones de ESP-IDF y FreeRTOS como:
    esp_wifi_init() / esp_wifi_set_mode() / esp_wifi_set_config() / esp_wifi_start() - Para configurar y iniciar el AP WiFi.
    esp_event_handler_register() - Para registrar manejadores de eventos WiFi.
    gpio_set_direction() / gpio_set_level() - Para controlar los LEDs.
    xTaskCreatePinnedToCore() - Para crear tareas asignadas a núcleos específicos.
    vTaskDelete() - Para eliminar la tarea actual al finalizar su ejecución.
    vTaskDelay() - Para pausar la ejecución en el loop principal.
    ESP_LOGI() - Para imprimir logs en consola.
    nvs_flash_init() - Para inicializar el almacenamiento no volátil.RESULTADO

WIFI AP: Punto de acceso iniciado. SSID:Jueguru, Contraseña:Jueguru123, Canal:4, Max Conexiones:4, Modo Autenticacion:3
WIFI AP: Estación conectada. Conexiones Actuales: 1
WIFI AP: Estación conectada. Conexiones Actuales: 2
WIFI AP: Alguien se desconectó. Conexiones Actuales: 1

Los LEDs se encienden secuencialmente: el primer LED (GPIO 2) se activa con al menos 1 conexión, y el segundo (GPIO 4) con al menos 2 conexiones.NOTAS IMPORTANTESEs necesario incluir las bibliotecas adecuadas para WiFi y eventos: #include "esp_wifi.h", #include "esp_event.h", y para FreeRTOS: #include "freertos/FreeRTOS.h".
La variable conexiones es global y se declara en configuracion_wifi.c para acceso desde luces.c mediante extern.
Asegúrate de que los pines de LEDs (GPIO_NUM_2 y GPIO_NUM_4) estén disponibles en tu placa ESP32; ajusta si es necesario.
Para el manejo de errores, se usa ESP_ERROR_CHECK() para verificar operaciones críticas.

VIDEOSe evidencia la secuencia hecha: al conectar dispositivos al AP, los LEDs se encienden según el número de conexiones, y los logs muestran los eventos en tiempo real.

