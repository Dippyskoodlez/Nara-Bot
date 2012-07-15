#include <SoftwareSerial.h>

SoftwareSerial xBeeSerial(2, 3);

void setup()  
{



  xBeeSerial.begin(9600);
  xBeeSerial.println("Hello, world?");
}

void loop() // run over and over
{
  if (xBeeSerial.available()){
    Serial.write(xBeeSerial.read());
  }
  delay(300);
}
