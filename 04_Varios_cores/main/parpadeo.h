#ifndef PARPADEO_H
#define PARPADEO_H
#include "freertos/queue.h"

extern QueueHandle_t cola_contador;
void inicia_parpadeo(QueueHandle_t cola);

#endif