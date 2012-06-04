import processing.serial.*;
import procontroll.*;
import net.java.games.input.*;

Serial Xbee; //Name of our output via serial
double SendInterval = 100; //How often to send, changed or not
double CamInterval = 150;
double LastSend; //Global variable to track how long since our last transmit
double LastCamSend;
double CurrentTime;
int axisX;
int axisY;
int lastaxisX;
int lastaxisY;
int axisRX;
int axisRY;
int lastaxisRX;
int lastaxisRY;
int button0;

ControllIO controllIO;
ControllDevice joypad; //joypad references your joypad as a whole.
// --------------------- XBOX 360 Controls -------------
// ControllCoolieHat hat1;
 ControllSlider sliderX; //XBOX Left Slider X Axis
 ControllSlider sliderY; //XBOX Left Slider Y Axis
 ControllSlider sliderRX; //XBOX Right Slider X Axis
 ControllSlider sliderRY; //XBOX Right Slider Y Axis
 
 /*---------------------- PS3 --------------------------
ControllStick stickXY; //PS3 ONLY
ControllStick stickRZ; //PS3 ONLY
*/ 

ControllButton button0x;
//int[] buttonstates = new int[10];
//int[] lastSent = new int [10];

// -------SETUP ------
void setup(){
  Xbee = new Serial(this, Serial.list()[0],9600);
  controllIO = ControllIO.getInstance(this);
  /* ------------------------PS3 Settings ------------
  joypad = controllIO.getDevice("PLAYSTATION(R)3 Controller"); //PS3
  stickXY = joypad.getStick(0);  //PS3 ONLY
  stickRZ = joypad.getStick(1); //PS3 ONLY
  stickXY.setMultiplier(9); //PS3
  stickXY.setTolerance(0.08f); // PS3
  */
  // --------------------- XBOX Controls -------------
  joypad = controllIO.getDevice("Controller"); //xbox 360 controller detection. 
  sliderX = joypad.getSlider(0);  // Left Slider X axis
  sliderY = joypad.getSlider(1);  // Left Slider Y Axis
  sliderRX = joypad.getSlider(2); // Right Slider X Axis
  sliderRY = joypad.getSlider(3); // Right Slider Y Axis
  sliderX.setMultiplier(9); //this will change based on your controller, see proControll library references for more
  sliderY.setMultiplier(9); 
  sliderRX.setMultiplier(10);
  sliderRY.setMultiplier(10);
 joypad.setTolerance(0.10f);// Deadzone change based on your preferences/hardware
 //---------------------------------------------------
  button0x = joypad.getButton(0);
}


// ------ MAIN LOOP ------
void draw(){
//Xbee.write("B");
//Xbee.write(1);
//Xbee.write(2);
//delay(1500);
CurrentTime = millis();
ReadController();
SendData();
ReceiveData();
}
// -------- GET CONTROLLER STATE -------
void ReadController(){
  
  /* ----------------- PS3 STICKS--------------------
  axisX = int(stickXY.getY() ); //PS3 LEFT STICK
  axisY = int(-stickXY.getX()); //PS3 Left Stick XY
  axisRX = int(stickXY.getY()); //PS3 Left Stick XY
  axisRY = int(-stickXY.getX()); //PS3 Left Stick XY
  */
  //-------------------XBOX 360 STICKS --------------
axisX = int(sliderX.getValue()); // XBOX Left Stick X
axisY = int(-sliderY.getValue()); //XBOX Left Stick Y
axisRX = int(sliderRX.getValue()); //XBOX Right Stick X
axisRY = int(-sliderRY.getValue()); //XBOX Right Stick Y
//----------------------------------------------------

button0x = joypad.getButton(0);
button0 = int(button0x.getValue());
// DEBUG AXIS PRINTING ---
println(axisX);
println(axisY);
println();
// END DEBUG PRINTING ---
}
// ------ SEND THAT DATA, YO ----------
void SendData(){
//compare last sent array to current
if(axisX != lastaxisX || axisY != lastaxisY){
    SendCommand(axisX,axisY,axisRX);
    lastaxisX = axisX;
    lastaxisY = axisY;
    LastSend = CurrentTime;
    //Debug: This line tells you if it is reading the control pad correctly.
    //println("DIFFERENT");
    }
  else if((CurrentTime - LastSend) > SendInterval){
    if(axisX == lastaxisX && axisY == lastaxisY){
    SendCommand(axisX,axisY,axisRX);
    lastaxisX = axisX;
    lastaxisY = axisY;
    LastSend = CurrentTime;
    //Debug: This line tells you if it is reading the control pad correctly.
    //println("SAME");
    
    }
  }
/*  if(button0 > 0){
    delay(10);
  Xbee.write('h');
  Xbee.write(0);
  Xbee.write(0);
  }
*/
  
/*if(axisRX != lastaxisRX || axisRY != lastaxisRY){
    SendCam(axisRX,axisRY);
    lastaxisRX = axisRX;
    lastaxisRY = axisRY;
    LastCamSend = CurrentTime;
    //println("DIFFERENT");
    }
  else if((CurrentTime - LastCamSend) > CamInterval){
    if(axisRX == lastaxisRX && axisRY == lastaxisRY){
    SendCommand(axisRX,axisRY);
    lastaxisRX = axisRX;
    lastaxisRY = axisRY;
    LastCamSend = CurrentTime;
    //println("SAME");
    
    }
  }
  */
}
void SendCommand(int value1, int value2, int value3){
  
  Xbee.write('F');
      Xbee.write(value1);
      println(value1);
      Xbee.write(value2);
      println(byte(value2));
     SendCam(axisRX);
 // if(value1 == 0){
  //  if(value2 == 0){
   //   Xbee.write('x');
   //   Xbee.write(value1);
     // println('x');
  //    Xbee.write(value2);
     // println(value2);
  //   SendCam(axisRX);
 //   }
//  }
//  if(value1 == 0){
  //  if(value2 >= 0){
   //   Xbee.write('F');
   //   Xbee.write(0);
      //println(value1);
   //   Xbee.write(value2);
      //println(byte(value2));
   //   SendCam(axisRX);
  //  }
 //   if(value2 < 0){
  //    Xbee.write('B');
 //     Xbee.write(0);
  //    Xbee.write(value2); //NEG 9 for BACK
  //    SendCam(axisRX);

 
  //  }
 // }
 // else if(value1 < 0){
 //  if(value2 >= 0){
  //  Xbee.write('L');
  //  Xbee.write(-value1); 
 //   Xbee.write(value2);
  //  SendCam(axisRX);
  //  }
   // if(value2 < 0){
      //QUADRANT 4
  //  Xbee.write('l');
 //   Xbee.write(-value1); 
  //  Xbee.write(value2);  //NEG
 //   SendCam(axisRX);
   // }
 // }
 // else if(value1 > 0){
 //  if(value2 >= 0){
  //  Xbee.write('R');
 //   Xbee.write(value1);
  //  Xbee.write(value2);
 //   SendCam(axisRX);

  //  }
  //  if(value2 < 0){
      //QUADRANT 3
  //  Xbee.write('r');
  //  Xbee.write(value1);
  //  Xbee.write(value2); //NEG
  //  SendCam(axisRX);

  //  }
 // }
}

void SendCam(int value1){
  if(value1 < 0){
  Xbee.write((75-(-value1*6)));
  //println(75-(-value1*6));
  }
  else if(value1 >= 0){
    Xbee.write((75+(value1)*10));
  // println(75+(value1)*10);
  }
}

void ReceiveData(){
if (Xbee.available() > 0) {
    int inByte = int(Xbee.read());
    println(char(inByte));
  }
}
