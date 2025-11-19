#include <string.h> 
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h" // Include the Wi-Fi driver
#include "esp_event.h" // Include the event loop library
#include "esp_log.h"
#include "nvs_flash.h"
#include "luces.h"
#include "configuracion_wifi.h"


void app_main(void)
{
    luces_init();
    inicia_wifi();

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000)); 
    }
    
}