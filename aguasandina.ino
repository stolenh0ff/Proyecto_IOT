#include <SPI.h>
#include <Ethernet.h>
#include <Servo.h>
int keyIndex = 0;         
int SensorPin = A0;

Servo servoMotor;
byte mac[] = { 
0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(169,254,67,2);
EthernetServer server(80);

void setup() {
  Serial.begin(9600);
  servoMotor.attach(9);
  pinMode(9,OUTPUT);
  Serial.begin(9600);
Ethernet.begin(mac, ip);
server.begin();
}

void loop() {

  int humedad = analogRead(SensorPin);
  Serial.print(humedad);
if(humedad>=460)

{
 digitalWrite(9,LOW);
 servoMotor.write(0); 
      
}
else 
{  
      digitalWrite(9,HIGH);
      servoMotor.write(90);
}
  EthernetClient cliente = server.available(); // Inicializa cliente como servidor ethernet
  if (cliente) {
       boolean currentLineIsBlank = true;
while (cliente.connected()) {
  if (cliente.available()) {
       char c = cliente.read();
  if (c == '\n' && currentLineIsBlank) { 
          cliente.println("HTTP/1.1 200 OK");
          cliente.println("Content-Type: text/html");
          cliente.println("Connection: close"); 
          cliente.println("Refresh: 3");
          cliente.println();
          cliente.println("<!DOCTYPE HTML>"); 
          cliente.println("<html>");
          cliente.println("<HEAD>");
          cliente.println("<TITLE>Ethernet Monitor</TITLE>");
          cliente.println("</HEAD>");
          cliente.println("<BODY>");
          cliente.println("<hr />");
          cliente.println("<H1>Monitor de agua</H1>");
          cliente.println("<br />");  
          cliente.println("<H2>Monitorea pin N9</H2>");
          cliente.println("<br />");  
          cliente.println("monitoreo por medio de cable de red");  
          cliente.println("<br />");
          cliente.println("<br />");
  for (int puertoAnalogo = 9; puertoAnalogo < 10; puertoAnalogo++) {
       int lecturaSensor = analogRead(puertoAnalogo);
            cliente.print("Entrada Analoga ");
            cliente.print(puertoAnalogo);
            cliente.print(" es ");
            cliente.print(lecturaSensor);
            cliente.println("<br />");       
}
          cliente.println("<br />"); 
          cliente.println("</html>");
          break;
}
if (c == '\n') 
{
currentLineIsBlank = true;
} 
else if (c != '\r') {
currentLineIsBlank = false;
        }
      }
    }
delay(1); 
cliente.stop();
  }
}