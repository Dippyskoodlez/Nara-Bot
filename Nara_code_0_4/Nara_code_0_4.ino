//This sketch is designed for the Arduino Mega 2560.
//Created by Scott Billeg.
//Toasty Cat Productions, 2012
  //-------------------------------------------------------// 
 #include <SoftwareSerial.h> //Software serial over digital ports.
  #include <PololuQik.h> //Pololu Motor driver library
  
 //-------------------------------------------------------//
 //Shield/Arduino specific definitions
  PololuQik2s12v10 qik(2, 3, 4); //TX, RX, Reset
 
  char incomingByte; //Reading the transmission in 4 byte queues, [char][int][int][int] (device, val, val, val)
  int incomingByte2; //Motor 0 setting (left)
  int incomingByte3; //Motor 1 setting (right)
  int incomingByte4; //Old servo byte. NYI. Just here to help keep things from breaking.
  int in1 = 0;
 //-------------------------------------------------------// 
void  setup(){
  //Starts the serial connection, attaches the servo, and sets the ping timer to ~100ms
  Serial.begin(9600);
  
  qik.init(9600); //Start the qik serial connection.

  Serial.println("Good morning, Dave");
}
//-------------------------------------------------------// 
void loop(){ 
  //Reads the incoming command.
  ReadIncoming();
  //Executes the command. Fetch boy, fetch!
  ExecuteCommands();

}
//-------------------------------------------------------// 
void ReadIncoming(){
  if (Serial.available() > 3) {
    //Reads 4 bytes of serial buffer.
        incomingByte = Serial.read(); //Direction (FWD/REV)
        incomingByte2 = Serial.read(); //motor0
        incomingByte3 = Serial.read(); //motor1
        incomingByte4 = Serial.read(); //Servo Position
                
  }
  else if(Serial.available() < 4){
    //If the buffer doesn't have a complete command, it gets the flush again.
    Serial.flush();
  }
           /* Uncomment these to debug sent/recieved data transmission (loopback)
              Serial.write(incomingByte4);
              Serial.println("I received:");
              Serial.println(incomingByte);
              Serial.println(incomingByte2);
              Serial.println(incomingByte3); */

  
}
//-------------------------------------------------------// 
void ExecuteCommands(){
  
     qik.setSpeeds(incomingByte2*6, incomingByte3*6); //Sets motor 0, then motor 1 with one command.
  
}


