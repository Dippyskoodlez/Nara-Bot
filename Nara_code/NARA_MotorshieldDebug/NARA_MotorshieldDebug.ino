#include <SoftwareSerial.h>
#include <PololuQik.h>

PololuQik2s12v10 qik(2, 3, 4);

void setup()  
{
  Serial.begin(9600);

  // set the data rate for the SoftwareSerial port
  qik.init(9600);
}

void loop() // run over and over
{
  qik.setM0Speed(100);
  qik.setM1Speed(100);
  Serial.println("GO");
  delay(1000);
  qik.setM0Speed(0);
  Serial.println("STOP");
  delay(1000);
}
