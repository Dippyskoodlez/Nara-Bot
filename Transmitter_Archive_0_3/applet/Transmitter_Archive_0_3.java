import processing.core.*; 
import processing.xml.*; 

import processing.serial.*; 
import procontroll.*; 
import net.java.games.input.*; 

import java.applet.*; 
import java.awt.Dimension; 
import java.awt.Frame; 
import java.awt.event.MouseEvent; 
import java.awt.event.KeyEvent; 
import java.awt.event.FocusEvent; 
import java.awt.Image; 
import java.io.*; 
import java.net.*; 
import java.text.*; 
import java.util.*; 
import java.util.zip.*; 
import java.util.regex.*; 

public class Transmitter_Archive_0_3 extends PApplet {





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
ControllDevice joypad;
ControllCoolieHat hat1;
ControllSlider sliderX;
ControllSlider sliderY;
ControllSlider sliderRX;
ControllSlider sliderRY;
ControllButton button0x;
//int[] buttonstates = new int[10];
//int[] lastSent = new int [10];

// -------SETUP ------
public void setup(){
  Xbee = new Serial(this, Serial.list()[0],9600);
  controllIO = ControllIO.getInstance(this);
  joypad = controllIO.getDevice("Controller (Afterglow Gamepad for Xbox 360)");
  sliderX = joypad.getSlider(1);
  sliderY = joypad.getSlider(0);
 sliderRX = joypad.getSlider(3);
  sliderRY = joypad.getSlider(2);
  button0x = joypad.getButton(0);
  sliderX.setMultiplier(9); //this will change based on your controller, see proControll library references for more
  sliderY.setMultiplier(9);
  sliderRX.setMultiplier(10);
  sliderRY.setMultiplier(10);
joypad.setTolerance(0.10f); //deadzone basically, change based on your preferences/hardware
}


// ------ MAIN LOOP ------
public void draw(){
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
public void ReadController(){
axisX = PApplet.parseInt(sliderX.getValue());
axisY = PApplet.parseInt(-sliderY.getValue());
axisRX = PApplet.parseInt(sliderRX.getValue());
axisRY = PApplet.parseInt(-sliderRY.getValue());
button0x = joypad.getButton(0);
button0 = PApplet.parseInt(button0x.getValue());
//println(axisX);
//println(axisY);
}
// ------ SEND THAT DATA, YO ----------
public void SendData(){
//compare last sent array to current
if(axisX != lastaxisX || axisY != lastaxisY){
    SendCommand(axisX,axisY,axisRX);
    lastaxisX = axisX;
    lastaxisY = axisY;
    LastSend = CurrentTime;
    //println("DIFFERENT");
    }
  else if((CurrentTime - LastSend) > SendInterval){
    if(axisX == lastaxisX && axisY == lastaxisY){
    SendCommand(axisX,axisY,axisRX);
    lastaxisX = axisX;
    lastaxisY = axisY;
    LastSend = CurrentTime;
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
public void SendCommand(int value1, int value2, int value3){
  if(value1 == 0){
    if(value2 == 0){
      Xbee.write('x');
      Xbee.write(value1);
      //println('x');
      Xbee.write(value2);
     // println(value2);
     SendCam(axisRX);
    }
  }
  if(value1 == 0){
    if(value2 >= 0){
      Xbee.write('F');
      Xbee.write(0);
      //println(value1);
      Xbee.write(value2);
      //println(byte(value2));
      SendCam(axisRX);
    }
    if(value2 < 0){
      Xbee.write('B');
      Xbee.write(0);
      Xbee.write(-value2);
      SendCam(axisRX);
    }
  }
  else if(value1 < 0){
   if(value2 >= 0){
    Xbee.write('L');
    Xbee.write(-value1);
    Xbee.write(value2);
    SendCam(axisRX);
    }
    if(value2 < 0){
    Xbee.write('l');
    Xbee.write(-value1);
    Xbee.write(-value2);
    SendCam(axisRX);
    }
  }
  else if(value1 > 0){
   if(value2 >= 0){
    Xbee.write('R');
    Xbee.write(value1);
    Xbee.write(value2);
    SendCam(axisRX);
    }
    if(value2 < 0){
    Xbee.write('r');
    Xbee.write(value1);
    Xbee.write(-value2);
    SendCam(axisRX);
    }
  }
}

public void SendCam(int value1){
  if(value1 < 0){
  Xbee.write((75-(-value1*6)));
  //println(75-(-value1*6));
  }
  else if(value1 >= 0){
    Xbee.write((75+(value1)*10));
  // println(75+(value1)*10);
  }
}

public void ReceiveData(){
if (Xbee.available() > 0) {
    int inByte = PApplet.parseInt(Xbee.read());
    println(PApplet.parseChar(inByte));
  }
}
  static public void main(String args[]) {
    PApplet.main(new String[] { "--bgcolor=#FFFFFF", "Transmitter_Archive_0_3" });
  }
}
