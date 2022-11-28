#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Ethernet.h>

int keyIndex = 0;         
int SensorPin = A0;
String estado1="OFF";
String estado2="OFF";
Servo servoMotor;

byte mac[] = { 
0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(169,254,67,1);
EthernetServer server(80);
IPAddress gateway(169,254,67,0);
IPAddress netmask(255,255,255,0);

#define RST_PIN  9   //Pin 9 para el reset del RC522
#define SS_PIN  10   //Pin 10 para el SS (SDA) del RC522
#define LED_G 4
#define LED_R 5
MFRC522 mfrc522(SS_PIN, RST_PIN); ///Creamos el objeto para el RC522

void setup() {
  Serial.begin(9600); //Iniciamos La comunicacion serial
  servoMotor.attach(8);
  SPI.begin();        //Iniciamos el Bus SPI
  mfrc522.PCD_Init(); // Iniciamos el MFRC522
  Serial.println("Control de acceso:");

  pinMode(8,OUTPUT);

  pinMode(LED_G, OUTPUT);
  pinMode(LED_R, OUTPUT);

  Ethernet.begin(mac, ip);
server.begin();
}

byte ActualUID[4]; //almacenará el código del Tag leído
byte Usuario1[4]= {0x23 , 0x95, 0xEB, 0x11} ; //código del usuario 1
byte Usuario2[4]= {0x43, 0x62, 0xE6, 0x0B} ; //código del usuario 2

void loop() {
  // Revisamos si hay nuevas tarjetas  presentes
  if ( mfrc522.PICC_IsNewCardPresent()) 
        {  
      //Seleccionamos una tarjeta
            if ( mfrc522.PICC_ReadCardSerial()) 
            {
                  // Enviamos serialemente su UID
                  Serial.print(F("Card UID:"));
                  for (byte i = 0; i < mfrc522.uid.size; i++) {
                          Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
                          Serial.print(mfrc522.uid.uidByte[i], HEX);   
                          ActualUID[i]=mfrc522.uid.uidByte[i];          
                  } 
                  Serial.print("     ");                 
                  //comparamos los UID para determinar si es uno de nuestros usuarios  
                  if(compareArray(ActualUID,Usuario1))
            {
                    Serial.println("Acceso concedido...");
                    Serial.println();


                    digitalWrite(LED_G, HIGH);
                    delay(2000);
                    digitalWrite(8,HIGH);
                    servoMotor.write(90);
                    delay(7000);


                    digitalWrite(LED_G, LOW);
                    delay (2000);
            }
                   
                  else if(compareArray(ActualUID,Usuario2))
                  {
                    Serial.println("Acceso concedido...");
                    Serial.println();
                    delay(500);

                    digitalWrite(LED_G, HIGH);
                    delay(2000);
                    digitalWrite(8,HIGH);
                    servoMotor.write(90);

                    digitalWrite(LED_G, LOW);
                    delay (2000);
                  }
                  else {
                    Serial.println("Acceso denegado...");
                    digitalWrite(LED_R, HIGH);
                    delay(2000);
                    digitalWrite(LED_R, LOW);
                    delay(2000);

                  }
                  
                  // Terminamos la lectura de la tarjeta tarjeta  actual
                  mfrc522.PICC_HaltA();
          
            }
      
  }
int humedad = analogRead(SensorPin);

if(humedad>=460)
{
  digitalWrite(8,LOW);
  servoMotor.write(0);
  }
else
  {
    digitalWrite(8,HIGH);
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
          cliente.println("monitoreo wifi");  
          cliente.println("<br />");
          cliente.println("<br />");
  for (int puertoAnalogo = 8; puertoAnalogo < 9; puertoAnalogo++) {
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
 boolean compareArray(byte array1[],byte array2[])
{
  if(array1[0] != array2[0])return(false);
  if(array1[1] != array2[1])return(false);
  if(array1[2] != array2[2])return(false);
  if(array1[3] != array2[3])return(false);
  return(true);
}
