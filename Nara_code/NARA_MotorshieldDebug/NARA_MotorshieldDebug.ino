#include <SoftwareSerial.h>
#include <PololuQik.h>

PololuQik2s12v10 qik(2, 3, 4);
char spideysense;
byte axisX;
byte axisY;

void setup()  
{
  Serial.begin(9600);

  // set the data rate for the SoftwareSerial port
  qik.init(9600);
}

void loop() // run over and over
{
  while (Serial.available()>2) {
    //Reads 4 bytes of serial buffer.
       char spideysense = Serial.read(); //Direction (FWD/REV)
       axisX = Serial.read(); //motor0
       axisY = Serial.read(); //motor1
       // Uncomment these to debug sent/recieved data transmission (loopback)
              Serial.println("I received:");
              Serial.println(spideysense);
              Serial.println(axisX+axisY);
              Serial.println(axisY); //--
                
  }
 
  //else if(){
    //If the buffer doesn't have a complete command, it gets the flush again.

 // }
           
}
