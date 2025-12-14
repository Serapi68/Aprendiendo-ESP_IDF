//Crear server web

#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "esp_http_server.h"

// Función para iniciar el servidor web
httpd_handle_t web_server_start(void);

//Detener el servidor web

esp_err_t web_server_stop(httpd_handle_t server);

#endif