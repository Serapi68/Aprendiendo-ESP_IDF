//Sistema de autenticación para el servidor HTTP

#ifndef AUTH_H
#define AUTH_H
#include "esp_http_server.h"

//Puntero a la función de verificación de autenticación
esp_err_t check_auth(httpd_req_t *req);

//Acceso no autorizado
esp_err_t send_unauthorized_response(httpd_req_t *req);

#endif