#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include "esp_err.h"
#include "esp_netif_types.h"


// Función para inicializar el Wi-Fi en modo AP + STA
esp_err_t wifi_init_sta_ap(void);


// Detener la conexión Wi-Fi
esp_err_t wifi_stop(void);

#endif