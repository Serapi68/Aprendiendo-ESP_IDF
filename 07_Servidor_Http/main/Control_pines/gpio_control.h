//Control de pines GPIO

#ifndef GPIO_CONTROL_H
#define GPIO_CONTROL_H

#include "esp_err.h"
#include <stdbool.h>

typedef enum {
    LED_OFF = 0,
    LED_ON = 1
} led_state_t;

// Función para inicializar el GPIO del LED

esp_err_t gpio_led_init(void);

// Función para establecer el estado del LED
esp_err_t gpio_led_set(led_state_t state);

// Función para obtener el estado actual del LED
bool gpio_led_get_state(void);

#endif
