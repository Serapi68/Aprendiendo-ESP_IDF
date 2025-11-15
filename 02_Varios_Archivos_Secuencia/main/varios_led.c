#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"
#define boton GPIO_NUM_4
#define NUMERO_LEDS 3 //Cantidad de leds que se van a utilizar
const gpio_num_t LEDS[NUMERO_LEDS] = {GPIO_NUM_17,GPIO_NUM_16,GPIO_NUM_18}; //Array con los pines de los leds

void configurar_leds(void)
{
    gpio_set_direction(boton,GPIO_MODE_INPUT);
    gpio_set_pull_mode(boton, GPIO_PULLUP_ONLY);
    
    for (int i = 0; i < NUMERO_LEDS; i++)
    {
        gpio_set_direction(LEDS[i], GPIO_MODE_OUTPUT);
    }
    
    while(1)
    {
        int estado_boton = gpio_get_level(boton);
        
        // Si el botón cambió de estado , salir de la función
        if (estado_boton == 1)
        {
            // Apagar todos los LEDs al salir
            for (int i = 0; i < NUMERO_LEDS; i++)
            {
                gpio_set_level(LEDS[i], 0);
            }
            return; // Salir de la función
        }
        
        // Ejecutar un ciclo de secuencia
        for (int i = 0; i < NUMERO_LEDS; i++)
        {
            gpio_set_level(LEDS[i], 1);
            vTaskDelay(pdMS_TO_TICKS(500));
            gpio_set_level(LEDS[i], 0); 
        }
    }
}