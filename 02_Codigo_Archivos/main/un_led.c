#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#define LED GPIO_NUM_2
#define Boton GPIO_NUM_4

void manejo_led(void)
{
    gpio_set_direction(LED,GPIO_MODE_OUTPUT);
    gpio_set_direction(Boton,GPIO_MODE_INPUT);
    gpio_set_pull_mode(Boton, GPIO_PULLUP_ONLY);    

    while (1)
    {
        int estado_boton = gpio_get_level(Boton);
        
        // Si el botón cambió de estado, salir de la función
        if (estado_boton == 0)
        {
            printf("Botón presionado - Cambiando modo\n");
            gpio_set_level(LED, 0); 
            return; // Salir de la función
        }
        
        // Ejecutar ciclo de parpadeo
        gpio_set_level(LED,1);
        vTaskDelay(pdMS_TO_TICKS(1000)); 
        gpio_set_level(LED,0);
        vTaskDelay(pdMS_TO_TICKS(1000)); 
        printf("Hola Mundo\n");
    }
}