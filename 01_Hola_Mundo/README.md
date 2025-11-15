# _PRIMER CÓDIGO_

Primer paso para aprender a programar en ESP_IDF, inicialización con configuración de GPIO, uso de bibliotecas FreeRTOS y comunicación con monitor.

## SECUENCIA DE CÓDIGO

El código se ponen las librerías:

    #include <stdio.h>
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include "driver/gpio.h"
    #include "sdkconfig.h"

Se abre bucle app_main(void), como inicio de proceso se setea el modo de salida del GPIO

    void app_main(void)
    {
        gpio_set_direction(GPIO_NUM_2 ,GPIO_MODE_OUTPUT);

## SECUENCIA DE ACCIÓN 

La secuencia es sencilla, iniciamos con un print en el monitor para visualización del código y posible depuración o comprobación de un correcto _"flash"_ hacia el ESP_32, luego con ayuda de las funciones de timers variamos el tiempo de encendido y apagado de nuestro GPIO 2, que en ESP_32 tiene 
un LED integrado.

    while (1)
    {
        printf("Hola Mundo\n");
        gpio_set_level(GPIO_NUM_2 ,1);
        vTaskDelay(pdMS_TO_TICKS(1000)); 
        gpio_set_level(GPIO_NUM_2 ,0);
        vTaskDelay(pdMS_TO_TICKS(1000)); 

    }

