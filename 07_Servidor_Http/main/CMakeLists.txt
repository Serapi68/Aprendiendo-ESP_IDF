idf_component_register(
    SRCS 
        "main.c"
        "Autenticacion/auth.c"
        "Control_pines/gpio_control.c"
        "Configuracion_wifi/wifi_manager.c"
        "Server_http/web_server.c"
                        
    INCLUDE_DIRS 
        "."
        "Autenticacion"
        "Control_pines"
        "Configuracion_wifi"
        "Server_http"

    EMBED_FILES 
        "index.html"

    REQUIRES 
        mbedtls 
        esp_http_server 
        nvs_flash 
        esp_event
        esp_wifi
        driver
        esp_netif
        freertos
)