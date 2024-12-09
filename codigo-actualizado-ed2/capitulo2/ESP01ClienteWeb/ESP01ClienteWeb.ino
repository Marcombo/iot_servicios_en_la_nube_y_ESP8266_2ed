#include <ESP8266WiFi.h>                        
 
const char* ssid = "*************";                   // SSID de la red WIFI a la que desea conectarnos
const char* password = "*************";        //contraseña de dicha red

const char* servidorWeb = "www.google.com";
WiFiClient clienteWeb;            
 
void setup(){
  Serial.begin(115200);

  //Inicializa la conexión WIFI
  Serial.print("Conectando a " + String(ssid) + " ");     
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Conectado");
}
 
void loop(){                                
  //Establece la conexión con el servidor  
  if (clienteWeb.connect(servidorWeb, 80)){
    //Realiza la petición de la página HTML
    clienteWeb.print(String("GET /") + " HTTP/1.1\r\n" +
                 "Host: " + servidorWeb + "\r\n" +
                 "Connection: close\r\n" + "\n");
 
    //Lee la respuesta del servidor linea a linea
    while (clienteWeb.connected()){
      if (clienteWeb.available()){
        char caracter = clienteWeb.read();
        Serial.write(caracter);
      }
    }                                 
  }
  else Serial.println("Problemas de conexión con el servidor");
  
  delay(10000);
}
