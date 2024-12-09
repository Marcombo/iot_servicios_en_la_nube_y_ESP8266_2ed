#include <ESP8266WiFi.h>                        
 
const char* ssid = "*************";               // SSID de la red WIFI a la que desea conectarnos
const char* password = "*************";    //contraseña de dicha red
       
 
void setup()
{
  Serial.begin(115200);

  //Inicializa la conexión WIFI
  Serial.print("Conectando a " + String(ssid) + " ");     
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Conectado");
}
 
void loop(){}
