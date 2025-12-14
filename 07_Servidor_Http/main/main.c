/**  
 * @CREADO POR SERGIO ANDRES PILONIETA GARZON
 * @FECHA JUNIO 2024
 * @PROYECTO DE SERVIDOR HTTP CON AUTENTICACION BASICA Y CONTROL DE PINES GPIO
*/

//Librerias estandar
#include <stdio.h>
#include "Freertos/FreeRTOS.h"
#include "Freertos/task.h"
#include "esp_log.h"
#include "nvs_flash.h"

//Librerias del proyecto
#include "configuracion.h"
#include "wifi_manager.h"
#include "web_server.h"
#include "gpio_control.h"



//iniciar NVS

static esp_err_t nvs_init(void)
{
    esp_err_t ret = nvs_flash_init();
    
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_LOGW(TAG_MAIN, "NVS requiere formateo, borrando...");
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    
    if (ret != ESP_OK) {
        ESP_LOGE(TAG_MAIN, "Error al inicializar NVS: %s", esp_err_to_name(ret));
        return ret;
    }
    
    ESP_LOGI(TAG_MAIN, "NVS inicializado correctamente");
    return ESP_OK;
}


void app_main(void)
{
    ESP_LOGI(TAG_MAIN, "=================================");
    ESP_LOGI(TAG_MAIN, "  Iniciando ESP32 Web Server");
    ESP_LOGI(TAG_MAIN, "  Proyecto: Jueguru LED Control");
    ESP_LOGI(TAG_MAIN, "=================================");
    
    //1. inciar pin GPIO del LED
     ESP_LOGI(TAG_MAIN, "[1/4] Inicializando GPIO...");
    if (gpio_led_init() != ESP_OK) {
        ESP_LOGE(TAG_MAIN, "Error al inicializar GPIO. Abortando.");
        return;
    }

    //2. Iniciar NVS
    ESP_LOGI(TAG_MAIN, "[2/4] Inicializando NVS...");
    if (nvs_init() != ESP_OK) {
        ESP_LOGE(TAG_MAIN, "Error al inicializar NVS. Abortando.");
        return; 
    }
    

    //3. Iniciar Wi-Fi en modo AP + STA
    ESP_LOGI(TAG_MAIN, "[3/4] Inicializando Wi-Fi en modo AP + STA...");
    if (wifi_init_sta_ap() != ESP_OK) {
        ESP_LOGE(TAG_MAIN, "Error al inicializar Wi-Fi. Abortando.");
        return;
    }
    //4. Iniciar el servidor web
    ESP_LOGI(TAG_MAIN, "[4/4] Iniciando el servidor web...");
    httpd_handle_t server = web_server_start();
    if (server == NULL) {
        ESP_LOGE(TAG_MAIN, "Error al iniciar el servidor web. Abortando.");
        return;
    }
    ESP_LOGI(TAG_MAIN, "Servidor web iniciado correctamente.");
    ESP_LOGI(TAG_MAIN, "=================================================");
    ESP_LOGI(TAG_MAIN, "Opciones de conexión:");
    ESP_LOGI(TAG_MAIN, "  1) Conecta al AP '%s'", WIFI_SSID_AP);
    ESP_LOGI(TAG_MAIN, "     Contraseña: %s", WIFI_PASS_AP);
    ESP_LOGI(TAG_MAIN, "     Abre: http://192.168.4.1");
    ESP_LOGI(TAG_MAIN, "  2) Usa tu router WiFi '%s'", WIFI_SSID_STA);
    ESP_LOGI(TAG_MAIN, "     la ip asignada será mostrada en TAG_WIFI:");
    ESP_LOGI(TAG_MAIN, "==================================================");
    ESP_LOGI(TAG_MAIN, "Usuario: '%s'", AUTH_USERNAME);
    ESP_LOGI(TAG_MAIN, "Contraseña: '%s'", AUTH_PASSWORD);
    ESP_LOGI(TAG_MAIN, "==================================================");
    ESP_LOGI(TAG_MAIN, "Para generar una ip global se usara la aplicacion de ngrok");
}