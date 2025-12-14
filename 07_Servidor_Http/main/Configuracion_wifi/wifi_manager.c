#include "wifi_manager.h"
#include "configuracion.h"
#include <string.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"



static void event_handler_got_ip(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
    ESP_LOGI(TAG_WIFI, "IP asignada: " IPSTR, IP2STR(&event->ip_info.ip));
    ESP_LOGI(TAG_WIFI, "Abre en tu navegador: http://" IPSTR, IP2STR(&event->ip_info.ip));
    
}

static void event_handler_sta_start(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    ESP_LOGI(TAG_WIFI, "Intentando conectar a %s...", WIFI_SSID_STA);
    esp_wifi_connect();
}

static void event_handler_sta_disconnected(void* arg, esp_event_base_t event_base,
                                           int32_t event_id, void* event_data)
{
    wifi_event_sta_disconnected_t* event = (wifi_event_sta_disconnected_t*) event_data;
    ESP_LOGW(TAG_WIFI, "Desconectado del WiFi. Razón: %d", event->reason);
    ESP_LOGI(TAG_WIFI, "Reintentando conexión en 5 segundos...");
    
    vTaskDelay(pdMS_TO_TICKS(5000));
    esp_wifi_connect();
}

esp_err_t wifi_init_sta_ap(void){
    esp_err_t ret; 

    //inicializar el netif
    ret = esp_netif_init();
    if (ret != ESP_OK){
        ESP_LOGE(TAG_WIFI, "Error al inicializar el netif: %s", esp_err_to_name(ret));
        return ret;
    }

    //crear el loop de eventos
    ret = esp_event_loop_create_default();
    if (ret != ESP_OK){
        ESP_LOGE(TAG_WIFI, "Error al crear el loop de eventos: %s", esp_err_to_name(ret));
        return ret;
    }

    //Crear interfaces por defecto
    esp_netif_create_default_wifi_ap();
    esp_netif_create_default_wifi_sta();

    //inicializar el wifi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ret = esp_wifi_init(&cfg);
    if (ret != ESP_OK){
        ESP_LOGE(TAG_WIFI, "Error al inicializar el WiFi: %s", esp_err_to_name(ret));
        return ret;
    }

    //Configurar el modo AP
    wifi_config_t wifi_config_ap = {
        .ap = {
            .ssid = WIFI_SSID_AP,
            .ssid_len = strlen(WIFI_SSID_AP),
            .password = WIFI_PASS_AP,
            .max_connection = 5,
            .authmode = WIFI_AUTH_WPA2_PSK
        },
    };

    //Configurar el modo STA
    wifi_config_t wifi_config_sta = {
        .sta = {
            .ssid = WIFI_SSID_STA,
            .password = WIFI_PASS_STA,
        },
    };

    //Registrar manejadores de eventos
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,WIFI_EVENT_STA_START,
                                                        event_handler_sta_start,NULL,NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,WIFI_EVENT_STA_DISCONNECTED,
                                                        event_handler_sta_disconnected,NULL,NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,IP_EVENT_STA_GOT_IP,
                                                        event_handler_got_ip,NULL,NULL));  
                                                        
    //Establecer el modo WiFi a AP + STA
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config_ap));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config_sta));

    //Iniciar el WiFi
    ret = esp_wifi_start();
    if (ret != ESP_OK){
        ESP_LOGE(TAG_WIFI, "Error al iniciar el WiFi: %s", esp_err_to_name(ret));
        return ret;
    }
    ESP_LOGI(TAG_WIFI, "====================================");
    ESP_LOGI(TAG_WIFI, "  AP INICIADO: %s" , WIFI_SSID_AP);
    ESP_LOGI(TAG_WIFI, "  Contraseña: %s" , WIFI_PASS_AP);
    ESP_LOGI(TAG_WIFI, "  IP del AP: http://192.168.4.1");
    ESP_LOGI(TAG_WIFI, "====================================");
    ESP_LOGI(TAG_WIFI, "WiFi iniciado en modo AP + STA");
    ESP_LOGI(TAG_WIFI, "Conectando a: %s", WIFI_SSID_STA);

    return ESP_OK;

}

esp_err_t wifi_stop(void)
{
    esp_err_t ret = esp_wifi_stop();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG_WIFI, "Error al detener WiFi: %s", esp_err_to_name(ret));
    }
    return ret;
}