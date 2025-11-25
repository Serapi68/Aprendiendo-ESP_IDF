#include <stdio.h>
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "lwip/ip4_addr.h"
#include "lwip/lwip_napt.h" 
#include "esp_log.h"
#include "wifi_ap.h"
#include "wifi_sta.h"

static const char *TAG = "MAIN";

static void event_handler(void* arg, esp_event_base_t event_base,
                          int32_t event_id, void* event_data)
{
    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "CONECTADO AL ROUTER");
        ESP_LOGI(TAG, "IP del ESP32: " IPSTR, IP2STR(&event->ip_info.ip));

        // CORRECCIÓN: Obtener IP del AP correctamente
        esp_netif_t *ap_netif = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");
        esp_netif_ip_info_t ap_ip_info;
        esp_netif_get_ip_info(ap_netif, &ap_ip_info);  // Pasar por referencia
        
        // Activar NAT con la IP del AP
        ip_napt_enable(ap_ip_info.ip.addr, 1);
        ESP_LOGI(TAG, " NAT ACTIVADO - IP: " IPSTR, IP2STR(&ap_ip_info.ip));

        // Configurar DNS del AP = DNS del router 
        esp_netif_dns_info_t dns;
        dns.ip.type = ESP_IPADDR_TYPE_V4;
        dns.ip.u_addr.ip4.addr = event->ip_info.gw.addr;  // Gateway del router
        esp_netif_set_dns_info(ap_netif, ESP_NETIF_DNS_MAIN, &dns);
        
        // DNS secundario (Google DNS como backup)
        dns.ip.u_addr.ip4.addr = ESP_IP4TOADDR(8, 8, 8, 8);
        esp_netif_set_dns_info(ap_netif, ESP_NETIF_DNS_BACKUP, &dns);

        ESP_LOGI(TAG, " DNS configurado ");
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Iniciando ESP32 en modo AP+STA con Internet compartido");

    // 1. NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // 2. TCP/IP + Event loop
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // 3. Registrar evento para cuando tenga IP
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    // 4. Wi-Fi
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    // 5. Iniciar STA y AP 
    WIFI_STA_START();   // Primero el cliente
    WIFI_AP_START();    // Luego el punto de acceso

    ESP_LOGI(TAG, "Esperando conexión al router para activar NAT...");
    ESP_LOGI(TAG, "Conéctate al AP 'Jueguru' con contraseña 'Jueguru123'");
}