import processing.serial.*;
import procontroll.*;
import net.java.games.input.*;

Serial Xbee;               //Name of our output via serial
double SendInterval = 100; //How often to send, changed or not
double CamInterval = 150;
double LastSend;           //Global variable to track how long since our last transmit
double LastCamSend;
double CurrentTime;
int axisX;
int axisY;
int motor0;
int motor1;
int lastmotor0;
int lastmotor1;
int axisRX;
int axisRY;
int lastaxisRX;
int lastaxisRY;
int invertaxis;
char spideysense;
int button0;

ControllIO controllIO;
ControllDevice joypad;     //joypad references your joypad as a whole.
// --------------------- XBOX 360 Controls -------------
// ControllCoolieHat hat1;
 ControllSlider sliderX;   //XBOX Left Slider X Axis
 ControllSlider sliderY;   //XBOX Left Slider Y Axis
 ControllSlider sliderRX;  //XBOX Right Slider X Axis
 ControllSlider sliderRY;  //XBOX Right Slider Y Axis
 
 /*---------------------- PS3 --------------------------
ControllStick stickXY;     //PS3 ONLY
ControllStick stickRZ;     //PS3 ONLY
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
  stickXY = joypad.getStick(0);    //PS3 ONLY
  stickRZ = joypad.getStick(1);    //PS3 ONLY
  stickXY.setMultiplier(9);        //PS3
  stickXY.setTolerance(0.08f);     // PS3
  */
  // --------------------- XBOX Controls -------------
  joypad = controllIO.getDevice("Controller"); //xbox 360 controller detection. 
  sliderX = joypad.getSlider(0);  // Left Slider X axis
  sliderY = joypad.getSlider(1);  // Left Slider Y Axis
  sliderRX = joypad.getSlider(2); // Right Slider X Axis
  sliderRY = joypad.getSlider(3); // Right Slider Y Axis
  sliderX.setMultiplier(9);       //this will change based on your controller, see
  sliderY.setMultiplier(9);       // proControll library references for more
  sliderRX.setMultiplier(10);
  sliderRY.setMultiplier(10);
 joypad.setTolerance(0.10f);      // Deadzone change based on your preferences/hardware
 //---------------------------------------------------
 
  button0x = joypad.getButton(0);
}


// ------ MAIN LOOP ------
void draw(){

CurrentTime = millis();

ReadController(); //Reads controller state.
ProcessController360(); // Translates controller state to the proper Qik motor speeds. 
// ProcessControllerPS3(); // Translates controller state to the proper Qik motor speeds. 
SendData();       // Sends the bot the information.
ReceiveData();    //Recieves necessary data.

}
// -------- GET CONTROLLER STATE -------
void ReadController(){
  
  /* ----------------- PS3 STICKS--------------------
  axisX = int(stickXY.getY() ); //PS3 LEFT STICK 
  axisY = int(-stickXY.getX()); //PS3 Left Stick
  axisRX = int(stickXY.getY()); //PS3 Left Stick 
  axisRY = int(-stickXY.getX()); //PS3 Left Stick
  */
//-------------------XBOX 360 STICKS --------------
  axisX = int(sliderX.getValue()); // XBOX Left Stick X (<- [-9] --- [9] -> ) 
  axisY = int(-sliderY.getValue()); //XBOX Left Stick Y (Inverted, because UP reads -9.)
  axisRX = int(sliderRX.getValue()); //XBOX Right Stick X (<- [-9] --- [9] -> )
  axisRY = int(-sliderRY.getValue()); //XBOX Right Stick Y (Inverted, because UP reads -9.)
//----------------------------------------------------

button0x = joypad.getButton(0);
button0 = int(button0x.getValue());
/* DEBUG AXIS PRINTING ---
println(axisX);
println(axisY);
println();
//----END DEBUG PRINTING ---*/

} //CURLY BRACKET FOR READCONTROLLER()

void ProcessController360(){
  /*--------------------------------------
  Process the proper axis for sending motor speeds, based on quadrant read.
  These are the Values that need SENT for proper motion, over the READ Values.
                Y
         IV     |     I
    X:R Y:L     |  X:L Y:R
    X:-R  Y:L   |  X:L Y:R
 -X ----------------------- X
         III    |     II
     X:-R Y:-L  | X:-L Y:-R
     X:-R Y:-L  | X:L  Y:-R
               -Y
  ----------------------------------------*/
  //Motor 0 becomes byte 2 (Axis X) Motor 1 becomes byte 3 (Axis Y)
 
  //Quadrant I Logic
  if (axisX > 0 && axisY >0){
  spideysense = 'A';
  }
  //Quadrant II Logic
  if (axisX > 0 && axisY < 0){
   spideysense = 'B';
  }
  
  //Quadrant III Logic
  else if (axisX < 0 && axisY < 0) {
    spideysense = 'C';  
  }
  
  //Quadrant IV Logic
  else if (axisX < 0 && axisY > 0){
  spideysense = 'D';  
  }
  quadrantMaths();
  
  //Done! 
  }
  
void quadrantMaths(){
  
  switch(spideysense){
    case 'A': //Quadrant I
        //This is FWD/Right turning
        motor0 = axisY; //Left Full throttle speed.
        motor1 = axisY - axisX; //Reduce right motor by x Axis. 
        break;
    case 'B': //Quadrant II
        //Y is already negative; only need to change the X Value.
        motor0 = axisY;
        motor1 = axisY + axisX; 
        break;
    case 'C': //Quadrant III
        //This axis is like Q1, except negative.  Right faster than left, in reverse. Y > X
        motor0 = axisY - axisX; //NEGATIVES ADD, DUMMY. (-Y - -x) = (-Y + X)
        motor1 = axisY;
        break;
    case 'D': //Quadrant IV
       //axis X reads negative, but we're going forward, turning left. Adding a negative to a positive.
       motor0 = axisY + axisX;
       motor1 = axisY;
       break;
    
  }
  }

// ------ SEND THAT DATA, YO ----------
void SendData(){
//compare last sent array to current
if(motor0 != lastmotor0 || motor1 != lastmotor1){
    SendCommand(motor0,motor1,axisRX);
    lastmotor0 = motor0;
    lastmotor1 = motor1;
    LastSend = CurrentTime;
    //Debug: This line tells you if it is reading the control pad correctly. (Changing state)
    //println("DIFFERENT");
    }
  else if((CurrentTime - LastSend) > SendInterval){
    if(motor0 == lastmotor0 && motor1 == lastmotor1){
    SendCommand(motor0,motor1,axisRX);
    lastmotor0 = motor0;
    lastmotor1 = motor1;
    LastSend = CurrentTime;
    //Debug: This line tells you if it is reading the control pad correctly. (and seeing a constant state)
    //println("SAME");
    
    }
  }

}
void SendCommand(int value1, int value2, int value3){
  
  Xbee.write('F');
      Xbee.write(value1);
      println();
      print("LEFT:");
      print(value1);
      
      Xbee.write(value2);
      print("RIGHT:");
      print(value2);
     SendCam(axisRX);

}

// SERVO CODE (NYI)
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
