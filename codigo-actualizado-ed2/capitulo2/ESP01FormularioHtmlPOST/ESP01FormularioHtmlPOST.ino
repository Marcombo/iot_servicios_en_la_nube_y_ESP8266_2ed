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

  pinMode(0, OUTPUT);
  pinMode(2, OUTPUT);

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
            enviarMensajeRespuesta();
            break;
          }
          //La petición es de tipo POST
          else if (encabezado.startsWith("POST")) {
            //Se obtiene el tamaño de cuerpo
            String temp = encabezado.substring(encabezado.indexOf("Content-Length:") + 15);
            temp.trim();
            tamanioCuerpo = temp.toInt();
            //Se obtiene el cuerpo de la petición
            while(tamanioCuerpo-- > 0)
            {
              c = clienteWeb.read();
              Serial.write(c);
              cuerpo += c;
            }
            Serial.write("\n\n");
            controlGPIOs(cuerpo);
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

//Se asigna un estado a un gpio
void controlGPIOs(String cuerpo){
  int gpio = cuerpo.substring(cuerpo.indexOf("GPIO=")+5, (cuerpo.indexOf("&"))).toInt();
  int estado = cuerpo.substring(cuerpo.indexOf("Estado=")+7).toInt();
  
  digitalWrite(gpio, estado);
  
  if(estado == 0) resultadoEjecucion = "El GPIO" + String(gpio) + " se ha puesto a nivel bajo";
  else if(estado == 1) resultadoEjecucion = "El GPIO" + String(gpio) + " se ha puesto a nivel alto";
}

//Devuelve un mensaje de respuesta con una página HTML
void enviarMensajeRespuesta(){
  clienteWeb.println("HTTP/1.1 200 OK");
  clienteWeb.println("Content-Type: text/html");
  clienteWeb.println("Connection: close");
  clienteWeb.println();
  clienteWeb.println("<head>");
  clienteWeb.println("<title>Servidor Web ESP8266</title>");
  clienteWeb.println("</head>");
  clienteWeb.println("<html>");
  clienteWeb.println("<body>");
  clienteWeb.println("<form method='POST'>");
  clienteWeb.println("<fieldset style='width:300px'>");
  clienteWeb.println("<legend>Control GPIOs ESP-01</legend>");
  clienteWeb.println("GPIO: ");
  clienteWeb.println("<select name='GPIO'>");
  clienteWeb.println("<option value='0'>GPIO0</option>");
  clienteWeb.println("<option value='2'>GPIO2</option>");
  clienteWeb.println("</select>");
  clienteWeb.println("Estado: ");
  clienteWeb.println("<select name='Estado'>");
  clienteWeb.println("<option value='1'>ALTO</option>");
  clienteWeb.println("<option value='0'>BAJO</option>");
  clienteWeb.println("</select>");
  clienteWeb.println("<br><br>");
  clienteWeb.println("<input type='submit' value='Enviar'>");
  clienteWeb.println("</fieldset>");
  clienteWeb.println("</form>");
  clienteWeb.println("<br>");
  clienteWeb.println(resultadoEjecucion);
  clienteWeb.println("</body>");
  clienteWeb.println("</html>");
}

