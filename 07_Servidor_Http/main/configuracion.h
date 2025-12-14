//Configuracion de variables globales

#ifndef CONFIGURACION_H
#define CONFIGURACION_H

//Configuracion de pines
#define LED_GPIO 2

//Configuracion de red Wi-Fi AP
#define WIFI_SSID_AP "Jueguru"
#define WIFI_PASS_AP "12345678"

//Configuracion de red Wi-Fi STA
#define WIFI_SSID_STA "" // Coloca el SSID de red Wi-Fi aquí
#define WIFI_PASS_STA "" // Coloca la contraseña de red Wi-Fi aquí

//Credenciales para autenticación básica
#define AUTH_USERNAME "YoSoyJueguru" 
#define AUTH_PASSWORD "Jueguru" 
#define AUTH_REALM "ESP32 Web Server"

//Configuracion del puerto del HTTP
#define HTTP_PORT 80

// TAG para logs
#define TAG_MAIN "MAIN"
#define TAG_WEB "WEB"
#define TAG_WIFI "WIFI" 
#define TAG_AUTH "AUTH"
#define TAG_GPIO "GPIO" 

#endif 

