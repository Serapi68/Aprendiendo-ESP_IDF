#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#include "un_led.h"
#include "varios_led.h"

#define boton GPIO_NUM_4


void app_main(void)
{
    gpio_set_direction(boton, GPIO_MODE_INPUT);
    gpio_set_pull_mode(boton, GPIO_PULLUP_ONLY);
    
    int boton_estado_anterior = -1;
    
    while (1)
    {
        int boton_estado = gpio_get_level(boton);
        
        // Si el estado cambió, ejecutar la función correspondiente en loop
        if (boton_estado != boton_estado_anterior)
        {
            boton_estado_anterior = boton_estado;
        }
        
        // Ejecutar la función seleccionada continuamente
        if (boton_estado == 0)
        {
            configurar_leds();
        }
        else
        {
            manejo_led();
        }
    }
}