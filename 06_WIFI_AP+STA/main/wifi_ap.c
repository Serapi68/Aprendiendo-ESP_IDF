#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "dhcpserver/dhcpserver.h"  // ← AGREGAR

// Configuración del punto de acceso AP
#define Nombre "Jueguru"
#define Contraseña "Jueguru123"
#define Canal 0
#define Max_Conexiones 4
#define Modo_Autenticacion WIFI_AUTH_WPA_WPA2_PSK

static const char *TAG = "WIFI AP";

void WIFI_AP_START(void)
{
    esp_netif_t *ap_netif = esp_netif_create_default_wifi_ap();
    
    // 1. Detener DHCP temporalmente
    esp_netif_dhcps_stop(ap_netif);
    
    // 2. Configurar IP del AP
    esp_netif_ip_info_t ip_info;
    IP4_ADDR(&ip_info.ip, 192, 168, 4, 1);
    IP4_ADDR(&ip_info.gw, 192, 168, 4, 1);
    IP4_ADDR(&ip_info.netmask, 255, 255, 255, 0);
    esp_netif_set_ip_info(ap_netif, &ip_info);
    
    // 3. Configurar opciones DHCP para entregar DNS a los clientes
    dhcps_offer_t dhcps_dns_value = OFFER_DNS;
    esp_netif_dhcps_option(ap_netif, ESP_NETIF_OP_SET, ESP_NETIF_DOMAIN_NAME_SERVER, 
                          &dhcps_dns_value, sizeof(dhcps_dns_value));
    
    // 4. Configurar DNS primario (Google DNS)
    esp_netif_dns_info_t dns_info;
    dns_info.ip.type = ESP_IPADDR_TYPE_V4;
    dns_info.ip.u_addr.ip4.addr = ESP_IP4TOADDR(8, 8, 8, 8);
    esp_netif_set_dns_info(ap_netif, ESP_NETIF_DNS_MAIN, &dns_info);
    
    // 5. Configurar DNS secundario (Cloudflare DNS)
    dns_info.ip.u_addr.ip4.addr = ESP_IP4TOADDR(1, 1, 1, 1);
    esp_netif_set_dns_info(ap_netif, ESP_NETIF_DNS_BACKUP, &dns_info);
    
    // 6. Reiniciar DHCP con la nueva configuración
    esp_netif_dhcps_start(ap_netif);
    
    ESP_LOGI(TAG, "DHCP Server iniciado con DNS: 8.8.8.8 y 1.1.1.1");
    

    wifi_config_t wifi_ap_config = {
        .ap = {
            .ssid = Nombre,
            .ssid_len = strlen(Nombre),
            .channel = Canal,
            .password = Contraseña,
            .max_connection = Max_Conexiones,
            .authmode = Modo_Autenticacion,
            .ssid_hidden = 0, 
            .beacon_interval = 100,
        },
    };

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_ap_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Punto de acceso AP iniciado. SSID:%s, Contraseña:%s, Canal:%d, Max Conexiones:%d", 
             Nombre, Contraseña, Canal, Max_Conexiones);
}