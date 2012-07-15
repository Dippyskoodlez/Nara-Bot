//This sketch is designed for the Arduino Mega 2560.
//Created by Scott Billeg.
//Toasty Cat Productions, 2012
  //-------------------------------------------------------// 
 #include <SoftwareSerial.h> //Software serial over digital ports.
  #include <PololuQik.h> //Pololu Motor driver library
   #include <AFMotor.h> //Adafruit motor header library: http://www.adafruit.com/products/81
  
 //-------------------------------------------------------//
 //Shield/Arduino specific definitions
  PololuQik2s12v10 qik(2, 3, 4); //TX, RX, Reset
  AF_DCMotor motorL(1, MOTOR12_64KHZ);
  AF_DCMotor motorR(2, MOTOR12_64KHZ);
  AF_DCMotor motorL(3, MOTOR12_64KHZ); 
  AF_DCMotor motorR(4, MOTOR12_64KHZ);
 
  char spideysense; //Reading the transmission in 4 byte queues, [char][int][int][int] (device, val, val, val)
  int axisX; //Motor 0 setting (left)
  int axisY; //Motor 1 setting (right)
  int motor0;
  int motor1;
  int motortestvar = 40;
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
  quadrantMaths();
  

}
//-------------------------------------------------------// 
void ReadIncoming(){
  if (Serial.available()) {
    //Reads 4 bytes of serial buffer.
        spideysense = Serial.read(); //Direction (FWD/REV)
        axisX = Serial.read(); //motor0
        axisY = Serial.read(); //motor1
                
  }

  }
           /* Uncomment these to debug sent/recieved data transmission (loopback)
              Serial.println("I received:");
              Serial.println(spideysense);
              Serial.println(axisX);
              Serial.println(axisY); */

  

//-------------------------------------------------------// 

void quadrantMaths(){
  axisX = axisX * 6; //Left motor MAX = 9*x
  axisY = axisY* 6;  //Right motor MAX = 9*x
  
  switch(spideysense){
    case 'A': //Quadrant I
        //This is FWD/Right turning
        
        motor0 = axisY; //Left Full throttle speed.
        motor1 = axisY - axisX; //Reduce right motor by x Axis. 
      //  Serial.println(30);
      //  Serial.println(motor1);
      Serial.println("Setting Motor");
       qik.setM0Speed(motortestvar);
     // qik.setM1Speed(40);
        break;
    case 'B': //Quadrant II
        //Y is already negative; only need to change the X Value.
        motor0 = axisY;
        motor1 = axisY + axisX;
       qik.setM0Speed(motor0);
       qik.setM1Speed(motor1); 
       // println(motor0);
       // println(motor1);
        break;
    case 'C': //Quadrant III
        //This axis is like Q1, except negative.  Right faster than left, in reverse. Y > X
        motor0 = axisY - axisX; //NEGATIVES ADD, DUMMY. (-Y - -x) = (-Y + X)
        motor1 = axisY;
        qik.setM0Speed(motor0);
        qik.setM1Speed(motor1);
      //  println(motor0);
      //  println(motor1);
        break;
    case 'D': //Quadrant IV
       //axis X reads negative, but we're going forward, turning left. Adding a negative to a positive.
       motor0 = axisY + axisX;
       motor1 = axisY;
       qik.setM0Speed(motor0);
       qik.setM1Speed(motor1);
      //  println(motor0);
      //  println(motor1);
       break;
       case 'E':
         qik.init(9600); //Reinit the Motor driver when DPAD up is pressed.
         break;
         
    
  }
  }


