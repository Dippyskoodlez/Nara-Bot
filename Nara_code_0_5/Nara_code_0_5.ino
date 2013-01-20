//This sketch is designed for the Arduino Mega 2560.
//Created by Scott Billeg.
//Toasty Cat Productions, 2012
//-------------------------------------------------------// 
/* MEGA 2560 PINOUT
Digital:
1-
2-
3-
4-
5-
6-
7-
8-
9- WEBCAM SERVO
10-

Analog:
1-
2-
3-
4-
5-
6-
7-
8-

Serial:
0 TX/RX: USB/xBee
1 TX/RX: (Digial 14/15) Qik 12v10
2 TX/RX:
3 TX/RX:
*/

#include <Servo.h>

Servo webCam; //Creates object for webcam control MAX: 8
int webPos = 0; //Stores webcam zeroize position.
/*

Legacy reference libraries. Softserial for Arduino Uno.
#include <SoftwareSerial.h> //Software serial over digital ports.
#include <PololuQik.h> //Pololu Motor driver library
#include <AFMotor.h> //Adafruit motor header library: http://www.adafruit.com/products/81
  
 */
 //-------------------------------------------------------// 
 void  setup(){
   
  Serial.begin(9600); //Initialize serial 0 (USB/xBEE)
  Serial1.begin(9600); //Start the qik motor driver serial connection.
  
  webCam.attach(9) // Sets pin 9 to Servo attachment.

  Serial.println("Good morning, Dave");
}

//-------------------------------------------------------// 
void loop(){ 
  //Reads the incoming command.
  readCommand();
  //Makes sure the read command is within the proper limits.
  proofCommand();
  //Sets the Motor values.
  setMotors();
  //Sets webcam Pivot
  setCam();
  
}
//-------------------------------------------------------// 
void readCommand(){
  if (Serial.available()){
    //read each byte of serial buffer.
   //Motor 0 (-127/127)
   //Motor 1 (-127/127)
   //Servo 0 (On/Off)
   //Servo 1 (On/Off)
   //Servo 2 (On/Off)
   //Servo 3 (On/Off)
   //Servo 4 (On/Off)
   //Webcam Servo 0 (-90/90)
   //Webcam servo 1 (-90/90)
}


//-------------------------------------------------------// 
void proofCommand(){
}



//-------------------------------------------------------// 
void setMotors(){
  //Sets the Qik12v10 (Main drive) motor controller.
  setQikMotor();
  //Sets Adafruit Servo driver.
  setAdaMotor();
  
}
//-------------------------------------------------------// 
void setCam(){
  //Sets the webcam position according to webPos integer. (-90/90 expected.)
  webCam.write(webPos);
}
