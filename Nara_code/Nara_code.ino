
  //-------------------------------------------------------// 

  #include <Servo.h>  //Arduino Servo library
//  #include <NewPing.h> //NewPing library: http://arduino.cc/forum/index.php?topic=106043.0
//  #include <SoftwareSerial.h> //Software serial over digital ports. NYI.
  #include <PololuQik.h> //Pololu Motor driver library
  
   //-------------------------------------------------------//
  //Shield/Arduino specific definitions
 // #define pingSpeed 400 //400ms pingspeed using newPing
  Servo myservo; //Servo connected to motorshield servo1 connector.
 // NewPing sonar1(12, 8, 500); //Sensor on pins 8 and 12.
 PololuQik2s12v10 qik(2, 3, 4); //TX, RX, Reset
 
//  unsigned long pingTimer1; 
  char incomingByte; //Reading the transmission in 4 byte queues, [char][int][int][int] (device, val, val, val)
  int incomingByte2;
  int incomingByte3;
  int incomingByte4 = 75; //This sets the servo to the "zeroized" [forward] position.
  int in1 = 0;
 //-------------------------------------------------------// 
void  setup(){
  //Starts the serial connection, attaches the servo, and sets the ping timer to ~100ms
  Serial.begin(9600);
  
  myservo.attach(10); //Servo attached to pin X
  
  Serial.println("qik 2s12v10 dual serial motor controller");
  
  qik.init();
  
  Serial.print("Firmware version: ");
  Serial.write(qik.getFirmwareVersion());
  Serial.println();
 // pingTimer1 = millis() + pingSpeed;
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
        incomingByte = Serial.read(); //Device indicator
        incomingByte2 = Serial.read(); //Setting value
        incomingByte3 = Serial.read(); //setting value
        incomingByte4 = Serial.read(); //Servo Position
                
        // if (millis() >= pingTimer1) {
          //  pingTimer1 += pingSpeed; // Make sensor 1 fire again 100ms later (pingSpeed)
          //  in1 = ((sonar1.ping_cm()+4)); //Change the additional value added to the measured distance
                                          //according to your device calibration.
            Serial.print(int(in1));      //returns the measured distance.
  }
  else if(Serial.available() < 4){
    //If the buffer doesn't have a complete command, it gets the flush again.
    Serial.flush();
  }
                    // Uncomment these to debug sent/recieved data transmission (loopback)
               // Serial.write(incomingByte4);
                Serial.println("I received:");
                Serial.println(incomingByte);
                Serial.println(incomingByte2);
                Serial.println(incomingByte3);

  
}
//-------------------------------------------------------// 
void ExecuteCommands(){
  
  myservo.write(incomingByte4); //Move the servo to the indicated location.
  
  //Translate the command into a direction. F/B: Fwd/reverse, L/R: Left, Right, l/r: reverse turn.
  switch(incomingByte){
    case 'F':
    GoStraight(incomingByte3);
      Serial.println("GOING STRAIGHT AT:");
      Serial.println(incomingByte3, DEC);
      break;
     case 'B':
        GoBack(incomingByte3);
        Serial.println("GOING STRAIGHT AT:");
        Serial.println(incomingByte3, DEC);
        break;
     case 'L':
        TurnLeftF(incomingByte2,incomingByte3);
        break;
     case 'R':
        TurnRightF(incomingByte2,incomingByte3);
        break;
      case 'l':
        TurnLeftB(incomingByte2,incomingByte3);
        break;
      case 'r':
        TurnRightB(incomingByte2,incomingByte3);
        break;
  }
}
//-------------------------------------------------------// 
//Set motor driver to appropriate fwd speeds.
// Motor speeds are between -127/127 for the qik. Adjust min max values accordingly. 14*9=126
void GoStraight(int FSpeed){
  //seperates stop and go command.  Min/Max motor speeds here.
  if(FSpeed > 0 && FSpeed < 10){
    qik.setM0Speed(FSpeed*14);
     qik.setM1Speed(FSpeed*14);
  }
  else if(FSpeed == 0){
    //STOP.
    qik.setM0Speed(0);
    qik.setM1Speed(0);
  }
}
//-------------------------------------------------------// 
void GoBack(int FSpeedB){
  //Same as fwd, except reverse.
  if(FSpeedB > 0 && FSpeedB < 10){
    qik.setM0Speed(FSpeedB*14);
    qik.setM1Speed(FSpeedB*14);
  }
  else if(FSpeedB == 0){
    //STOP.
   qik.setM0Speed(0);
   qik.setM1Speed(0);
  }
}
//-------------------------------------------------------// 
void TurnLeftF(int FSpeedM,int FSpeedB){
  //Left turn by reducing the left motor speed
  if (FSpeedB < 10 && FSpeedB > 0 && FSpeedM < 10 && FSpeedM > 0){
   qik.setM0Speed((FSpeedB*(14-(FSpeedM*2) )));
   qik.setM1Speed(FSpeedB*14);
  }
  //Neutral turn
  else if(FSpeedB == 0 && FSpeedM > 0 && FSpeedM < 10){
  qik.setM0Speed(FSpeedB*14);
  qik.setM1Speed(FSpeedB*14);
  }
}
//-------------------------------------------------------// 
void TurnRightF(int FSpeedM,int FSpeedB){
  //Right turn by reducing Right motors.
  if (FSpeedB < 10 && FSpeedB > 0 && FSpeedM < 10 && FSpeedM > 0){
   qik.setM0Speed(FSpeedB*14);
   qik.setM1Speed((FSpeedB*(14-(FSpeedM*2) )));
  }
  //Neutral steer right
  else if(FSpeedB == 0 && FSpeedM > 0 && FSpeedM < 10){
   qik.setM0Speed(FSpeedB*14);
   qik.setM1Speed(FSpeedB*14);
  }
}
//-------------------------------------------------------// 
void TurnLeftB(int FSpeedM,int FSpeedB){
  //Turns left while in reverse
  if (FSpeedB < 10 && FSpeedB > 0 && FSpeedM < 10 && FSpeedM > 0){
   qik.setM0Speed((FSpeedB*(14-(FSpeedM*2) )));
   qik.setM1Speed(FSpeedB*14);
  }
}
//-------------------------------------------------------// 
void TurnRightB(int FSpeedM,int FSpeedB){
  //Turn right while in reverse
  if (FSpeedB < 10 && FSpeedB > 0 && FSpeedM < 10 && FSpeedM > 0){
   qik.setM0Speed(FSpeedB*14);
   qik.setM1Speed((FSpeedB*(14-(FSpeedM*2) )));
  }
}
