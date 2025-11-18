#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "esp_idf_version.h"
#include "boton.h"
#include "parpadeo.h"
#include "contador.h"

QueueHandle_t contador_cola;
void app_main(void)
{   // Crear la cola para la comunicacion entre contador y parpadeo
       contador_cola = xQueueCreate(10, sizeof(int));
       inicia_parpadeo(contador_cola);
       inicia_contador(contador_cola);
       muestra_datos();

}