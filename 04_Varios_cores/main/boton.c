#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BOTON_PIN GPIO_NUM_0        

// Secuencia para el boton donde se hara una muestra por el monitor de ESP_IDF
// Del proceso de las otras dos tareas
void secuencia_boton(void *pvParameter)
{
    gpio_set_direction(BOTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BOTON_PIN, GPIO_PULLUP_ONLY);
    
    int ultimo_estado = 1;
    char buffer [500]; // 1 byte = 8 bits -> 500 bytes = 4000 bits 
    
    while(1){
    int estado_boton = gpio_get_level(BOTON_PIN);
        if (estado_boton == 0 && ultimo_estado == 1){
            printf("Mostrando tareas\n");
            printf("\n");
            printf("Nombre          Estado   Prioridad  Stack  Num   Core\n");
            printf("====================================================\n");
            vTaskList(buffer);
            printf("%s\n", buffer);
        }
    ultimo_estado = estado_boton;
    vTaskDelay(pdMS_TO_TICKS(100));    
    }

}

void muestra_datos(void)
{
  xTaskCreatePinnedToCore(
        secuencia_boton,        // Funcion de la tarea
        "muestra_datos",       // Nombre de task
        2048,                  // Tamaño de stack
        NULL,                  //  Parámetro de tarea
        3,                     // Prioridad de la tarea
        NULL,                  // Puntero a la tarea creada
        0);                    // Core 0 

}

