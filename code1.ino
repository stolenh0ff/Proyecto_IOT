#include <Servo.h>

 int SensorPin = A0;
 
Servo servoMotor;
 
void setup() {

  Serial.begin(9600);
 
    servoMotor.attach(9);
    pinMode(9,OUTPUT);
    Serial.begin(9600);
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
}