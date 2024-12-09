#include <ESP8266WiFi.h>
 
const char* ssid = "*************";             // SSID de la red WIFI a la que deseamos conectarnos
const char* password = "*************";  // contraseña de dicha red

IPAddress ip(192, 168, 1, 99);      //dirección IP elegida dentro de la red
IPAddress gateway(192, 168, 1, 1);  //dirección IP del gateway
IPAddress subnet(255, 255, 255, 0); //mascara de red
 
WiFiServer servidorWeb(80);
WiFiClient clienteWeb;
 
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
}
 
void loop() {
  clienteWeb = servidorWeb.available();
  //Se espera que llegue un cliente
  if (clienteWeb) {
    boolean nuevaLinea  = true;
    String encabezado = ""; 
    String cuerpo = ""; 
    int tamanioCuerpo = -1;

    //El cliente está conectado
    while (clienteWeb.connected()) {
      //El cliente ha transmitido datos
      if (clienteWeb.available()) {
        char c = clienteWeb.read();
        Serial.write(c);
        encabezado += c;
        //Fin del encabezado
        if (c == '\n' && nuevaLinea){
          if (encabezado.startsWith("GET")){
            enviarMensajeRespuesta();
            break;
          }
          if (encabezado.startsWith("POST")) {
            //Se obtiene el tamaño de cuerpo
            String temp = encabezado.substring(encabezado.indexOf("Content-Length:") + 15);
            temp.trim();
            tamanioCuerpo = temp.toInt();
            //Se obtiene el cuerpo de la petición
            while(tamanioCuerpo-- > 0){
              c = clienteWeb.read();
              Serial.write(c);
              cuerpo += c;
            }
            Serial.write("\n\n");
            enviarMensajeRespuesta();
            break;
          }
        }
        if (c == '\n') nuevaLinea  = true; 
        else if (c != '\r') nuevaLinea  = false; 
      }
    }
    delay(10);         
  }
}

//Devuelve un mensaje de respuesta con una página HTML
void enviarMensajeRespuesta(){
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
}

