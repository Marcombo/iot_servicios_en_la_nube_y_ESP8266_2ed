#include <ESP8266WiFi.h>
 
const char* ssid = "*************";             // SSID de la red WIFI a la que deseamos conectarnos
const char* password = "*************";  // contraseña de dicha red

IPAddress ip(192, 168, 1, 99);      //dirección IP elegida dentro de la red
IPAddress gateway(192, 168, 1, 1);  //dirección IP del gateway
IPAddress subnet(255, 255, 255, 0); //mascara de red
 
WiFiServer servidorWeb(80);
WiFiClient clienteWeb;

String resultadoEjecucion;
 
void setup() {
  Serial.begin(115200);

  pinMode(0, INPUT_PULLUP);
  pinMode(2, INPUT_PULLUP);

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
  resultadoEjecucion = "";
  clienteWeb = servidorWeb.available();
  //Se espera que llegue un cliente
  if (clienteWeb) {
    boolean nuevaLinea = true;
    String encabezado = ""; 
    String cuerpo = ""; 
    int tamanioCuerpo = -1;

    //El cliente está conectado
    while (clienteWeb.connected()) {
      //el cliente ha transmitido datos
      if (clienteWeb.available()) {
        char c = clienteWeb.read();
        Serial.write(c);
        encabezado += c;
        //Fin del encabezado
        if (c == '\n' && nuevaLinea){       
          //La petición es de tipo GET
          if (encabezado.startsWith("GET")){
            controlGPIOs(encabezado);
            enviarMensajeRespuesta();
            break;
          }
          //La petición es de tipo POST
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
        if (c == '\n') nuevaLinea = true; 
        else if (c != '\r') nuevaLinea = false; 
      }
    }
    delay(10);         
  }
}

//Se obtiene el valor del GPIO
void controlGPIOs(String encabezado){
  int gpio = encabezado.substring(encabezado.indexOf("GPIO=")+5, encabezado.indexOf("GPIO=")+6).toInt();
  int valor = digitalRead(gpio);
  if (valor == 1) resultadoEjecucion = "El GPIO" + String(gpio) + " tiene un valor ALTO";
  else resultadoEjecucion = "El GPIO" + String(gpio) + " tiene un valor BAJO";
}

//Devuelve un mensaje de respuesta con una página HTML
void enviarMensajeRespuesta(){
  clienteWeb.println("HTTP/1.1 200 OK");
  clienteWeb.println("Content-Type: text/html");
  clienteWeb.println("Connection: close");
  clienteWeb.println();
  clienteWeb.println("<head>");
  clienteWeb.println("<title>Control Web de led con ESP8266</title>");
  clienteWeb.println("<meta charset='UTF-8'>");
  clienteWeb.println("</head>");
  clienteWeb.println("<html>");
  clienteWeb.println("<body>");
  clienteWeb.println("<center>");
  clienteWeb.println("<h4>Pulse sobre el botón cuyo GPIO desea conocer su estado</h4>");
  clienteWeb.println("<button onClick=location.href='./?GPIO=0'>GPIO0</button>");
  clienteWeb.println("<button onClick=location.href='./?GPIO=2'>GPIO2</button>");
  clienteWeb.println("<br><br>");
  clienteWeb.println(resultadoEjecucion);
  clienteWeb.println("</center>");
  clienteWeb.println("</body>");
  clienteWeb.println("</body>");
  clienteWeb.println("</html>");

}

