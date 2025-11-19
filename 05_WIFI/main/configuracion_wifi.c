#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h" // Include the Wi-Fi driver
#include "esp_event.h" // Include the event loop library
#include "esp_log.h"
#include "nvs_flash.h"
#include "luces.h"  // Incluir para declarar actualizar_led

// Configuración del punto de acceso
#define Nombre "Jueguru"
#define Contraseña "Jueguru123"
#define Canal 4 // Numero de canal de acceso
#define Max_Conexiones 4 // Numero maximo de conexiones
#define Modo_Autenticacion WIFI_AUTH_WPA_WPA2_PSK // Tipo de autenticacion

static const char *TAG = "WIFI AP";
int conexiones = 0;  // Global para acceso desde otros archivos

static void wifi_eventos(void* arg, esp_event_base_t event_base,
                         int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        conexiones++;
        ESP_LOGI(TAG, "Estación conectada. Conexiones Actuales: %d", conexiones);
        actualizar_led(conexiones);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        if (conexiones > 0) {
            conexiones--;
            ESP_LOGI(TAG, "Alguien se desconectó. Conexiones Actuales: %d", conexiones);
            actualizar_led(conexiones);
        }
    }
}

static void wifi_init_ap(void *pvParameter)
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // Registrar el manejador de eventos
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT,ESP_EVENT_ANY_ID,&wifi_eventos,NULL));

    // Configurar el AP de esp_wifi
    wifi_config_t wifi_config = {
        .ap = {
            .ssid = Nombre,
            .ssid_len = strlen(Nombre),
            .channel = Canal,
            .password = Contraseña,
            .max_connection = Max_Conexiones,
            .authmode = Modo_Autenticacion
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Punto de acceso iniciado. SSID:%s, Contraseña:%s, Canal:%d, Max Conexiones:%d, Modo Autenticacion:%d",
             Nombre, Contraseña, Canal, Max_Conexiones, Modo_Autenticacion);
    vTaskDelete(NULL);
}

void inicia_wifi(void)
{
    xTaskCreatePinnedToCore(wifi_init_ap, "wifi_init_ap", 8192, NULL, 5, NULL, 1);
}