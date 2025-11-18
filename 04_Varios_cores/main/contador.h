#ifndef contador_H
#define contador_H
#include "freertos/queue.h"

extern QueueHandle_t cola_contador;
void inicia_contador(QueueHandle_t cola);

#endif
