#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

void app_main(void)
{
    gpio_set_direction(GPIO_NUM_2 ,GPIO_MODE_OUTPUT);
    while (1)
    {
        printf("Hola Mundo\n");
        gpio_set_level(GPIO_NUM_2 ,1);
        vTaskDelay(pdMS_TO_TICKS(1000)); 
        gpio_set_level(GPIO_NUM_2 ,0);
        vTaskDelay(pdMS_TO_TICKS(1000)); 

    }
    

}