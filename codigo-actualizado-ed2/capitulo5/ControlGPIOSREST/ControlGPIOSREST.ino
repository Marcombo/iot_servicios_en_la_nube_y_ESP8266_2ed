#include <ESP8266WiFi.h>
#include <aREST.h>
 
const char* ssid = "*************";             // SSID de la red WIFI a la que deseamos conectarnos
const char* password = "*************";  // contraseña de dicha red

IPAddress ip(192, 168, 1, 99);      //dirección IP elegida dentro de la red
IPAddress gateway(192, 168, 1, 1);  //dirección IP del gateway
IPAddress subnet(255, 255, 255, 0); //mascara de red
 
WiFiServer servidorWeb(80);
WiFiClient clienteWeb;
//Se crea la instancia del API REST
aREST rest = aREST(); 
//Identificador y nombre del API
const char* api_id = "1";
const char* api_name = "WEMOS D1";
 
void setup() {
  Serial.begin(115200);

  //Se inicia la conexión WiFI
  Serial.print("Conectando a " + String(ssid) + " ");   
  WiFi.config(ip, gateway, subnet);  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Conectado");

  //Se arranca el servidor Web
  servidorWeb.begin();
  Serial.println("Servidor web arrancado");

  //Se asigna un nombre  y un identificador al API REST
  rest.set_id(api_id);
  rest.set_name(api_name);
}
 
void loop() {
  clienteWeb = servidorWeb.available();
  if (!clienteWeb) return;
  while(!clienteWeb.available()) delay(10);
  //Se atienden las peticiones HTTP
  rest.handle(clienteWeb);
}
