#include <ESP8266WiFi.h>
 
const char* ssid = "*************";             // SSID de la red WIFI a la que deseamos conectarnos
const char* password = "*************";  // contraseña de dicha red

IPAddress ip(192, 168, 1, 99); 
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
 
WiFiServer servidorWeb(80);
 
void setup() {
  Serial.begin(115200);

  Serial.print("Conectando a " + String(ssid) + " ");   
  WiFi.config(ip, gateway, subnet);  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Conectado");

  servidorWeb.begin();
  Serial.println("Servidor web arrancado");
}
 
void loop() {
  WiFiClient clienteWeb = servidorWeb.available();
  if (clienteWeb){
    clienteWeb.println("HTTP/1.1 200 OK");
    clienteWeb.println("Content-Type: text/html");
    clienteWeb.println("Connection: close");
    clienteWeb.println("");
    clienteWeb.println("<head>");
    clienteWeb.println("<title>Servidor Web ESP8266</title>");
    clienteWeb.println("</head>");
    clienteWeb.println("<html>");
    clienteWeb.println("<body>");
    clienteWeb.println("<h1>Bienvenido al servidor Web de mi ESP8266</h1>");
    clienteWeb.println("</body>");
    clienteWeb.println("</html>");
 
  delay(10);
  }
}

