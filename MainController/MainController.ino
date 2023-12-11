#include <Wire.h>
#include <Adafruit_NeoPixel.h>
byte data[4] = {0,0,0,30};

String submode = "startup";

int inputWDT = 3600000;
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(4, 12, NEO_GRB + NEO_KHZ800);

boolean freeMap[4] = {false,false,false,false};
const boolean FREE[4] = {true,true,true,true};
const boolean SAD[4] = {true,true,true,true};

int SensorWDT = 0;
int MotorWDT = 0;
int TelemWDT = 0;

int dist_fwd = 0;
int dist_left = 0;
int dist_right = 0;
int dist_back = 0;


int inputFWD = 0;
int inputSIDE = 0;
int inputROT = 0;
int inputMULT = 0;

int eurobeatCountdown = -1;  //DONT ASK
String allowed[4] = {"","","",""};
int sensorTimeMax = 100;
int  motorTimeMax = 100; 

int LoopTime = 0;
long loopMarker = 0;

int motorFWD = 0;
int motorROT = 0;
int motorSIDE = 0;
int motorMULT = 0;

long start = millis();

int DebugLevel = 4; //    -1 - 4 
String Mode = "IDLE";

void allowDir(String dir){
    if(dir.equals("none")){
        allowed[0] = "";
        allowed[1] = "";
        allowed[2] = "";
        allowed[3] = "";
        return;
    }
    for(int i = 0;i<4;i++){
        if(allowed[i].equals("")){
            allowed[i] = dir;
            return;
        }else if(allowed[i].equals(dir)){
            return;
        }
    }
    return;
}

void DevLog(String Message,String Origin, int level = 1){
    if(level >= DebugLevel){
        const String Codes[] = {"FORCED","ERROR","WARNING","SUCCESS","CONSOLE"};
        String ErrorCode = Codes[level];
        String msg = Origin+": "+ErrorCode+" -> "+Message;
        Serial1.print(msg);  //MotorCalc: ERROR -> NullPointerException
        Serial.print(msg);
    }
}


void setup() {
  pixels.begin();
  pixels.show();
  Wire.begin();
  Serial.begin(115200);
  Serial1.begin(115200);


  loopMarker = millis();
}

void loop() {
    LoopTime = millis()-loopMarker;
    loopMarker = millis();
    
    if(Serial1.available()){
        String msgi = Serial1.readString();
        HandleSerialIn(msgi);
    }
    if(Serial.available()){
        String msgi = Serial.readString();
        HandleSerialIn(msgi);
    }

    SensorWDT += LoopTime;
    if(SensorWDT > sensorTimeMax){
        if(getSensorVals()){
            SensorWDT = 0;
        }
    }

    motorMULT = 16;
    if(Mode == "IDLE"){
        motorFWD = 0;
        motorROT = 0;
        motorSIDE = 0;
    }else if(Mode == "AUTO"){
        Serial1.println(submode);
        if(submode.equals("startup")){
            motorFWD = 0;
            motorROT = 0;
            motorSIDE = 0;
                     //fwd, back,left,right
            setFreeMap(FREE);
            ezmap();
            if(freeMap == SAD){
                submode = "sad";
            }else{
                submode = "check";
                start = millis();
            }
        }else if(submode.equals("forward")){
           
        }else if(submode.equals("backup")){

        }else if(submode.equals("left")){
            
        }else if(submode.equals("right")){
            
        }else if(submode.equals("check")){
            #define rotTime 2000
            setFreeMap(FREE);
            if(millis()<start+rotTime){
                motorFWD = 0;
                motorMULT = 20;
                motorROT = -64;
                motorSIDE = 0;
            }else if(millis()<start+(rotTime*3)){
                motorFWD = 0;
                motorMULT = 20;
                motorROT = 64;
                motorSIDE = 0;
                ezmap();
            }else if(millis()<start+(rotTime*4)){
                motorFWD = 0;
                motorMULT = 20;
                motorROT = -64;
                motorSIDE = 0;
                
            }else if(millis()<start+(rotTime*5)){
                submode = "decide";
            }
        }else if(submode.equals("honk")){
            //honk
        }else if(submode.equals("sad")){
            //playingsound("sad");
        }else if(submode.equals("decide")){
            if(freeMap[0]){
                submode = "forward";
            }else{
                allowDir("none");
                int n = 0;
                if(freeMap[1]){
                    allowDir("backup");
                    n++;
                }
                if(freeMap[2]){
                    allowDir("left");
                    n++;
                }
                if(freeMap[3]){
                    allowDir("right");
                    n++;
                }
                if(n == 0){
                    submode = "sad";
                }else{
                    submode = allowed[random(n-1)];
                }
            }
        }else if(submode.equals("a")){
            
        }else{
            DevLog("no valid submode detected","AUTO_MODE",3);
        }
    }else if(Mode == "MANUAL"){
        motorFWD = 128;
        motorMULT = 30;
        motorROT = 0;
        motorSIDE = 0;
        if(inputWDT < 500){
            motorFWD = inputFWD;
            motorMULT = inputMULT;
            motorROT = inputROT;
            motorSIDE = inputSIDE;
        }else{
            motorFWD = 0;
            motorMULT = 0;
            motorROT = 0;
            motorSIDE = 0;
        }
    }else if(Mode == "RAMPAGE1"){
        motorFWD = 0;
        motorMULT = 255;
        motorROT = 127;
        motorSIDE = 0;
    }else if(Mode == "RAMPAGE2"){
        motorFWD = 0;
        motorMULT = 127;
        motorROT = 127;
        motorSIDE = 0;
    }else if(Mode == "RAMPAGE3"){
        motorFWD = 0;
        motorMULT = 127;
        motorROT = 90;
        motorSIDE = 90;
    }else if(Mode == "RAMPAGE4"){
        motorFWD = 90;
        motorMULT = 127;
        motorROT = 90;
        motorSIDE = 0;
    }else if(Mode == "fwd"){
        motorFWD = 127;
        motorMULT = 255;
        motorROT = 0;
        motorSIDE = 0;
    }else if(Mode == "left"){
        motorFWD = 0;
        motorMULT = 255;
        motorROT = 0;
        motorSIDE = -127;
    }else if(Mode == "right"){
        motorFWD = 0;
        motorMULT = 255;
        motorROT = 0;
        motorSIDE = 127;
    }else if(Mode == "back"){
        motorFWD = -127;
        motorMULT = 255;
        motorROT = 0;
        motorSIDE = 0;
    }else if(Mode == "rleft"){
        motorFWD = 0;
        motorMULT = 255;
        motorROT = -127;
        motorSIDE = 0;
    }else if(Mode == "rright"){
        motorFWD = 0;
        motorMULT = 255;
        motorROT = 255;
        motorSIDE = 0;
    }



    MotorWDT += LoopTime;
    if(MotorWDT > motorTimeMax){
        if(setMotorSpeeds()){
            MotorWDT = 0;
        }
    }
    pixels.fill(pixels.Color(8,0,8));
    pixels.show();
}

void ezmap(){
    if(dist_back <= 20){
        freeMap[1] = false;
    }
    if(dist_fwd <= 20){
        freeMap[0] = false;
    }
    if(dist_left <= 20){
        freeMap[2] = false;
    }
    if(dist_right <= 20){
        freeMap[3] = false;
    }
}

boolean setMotorSpeeds(){
    data[0] = motorFWD+127;
    data[1] = motorSIDE+127;
    data[2] = motorROT+127;
    data[3] = motorMULT;
    Wire.beginTransmission(0x01);
    Wire.write(data,4);
    Wire.endTransmission();
    //Serial.println("sending...");
    //Serial1.println();
    //DevLog("0:"+String(data[0])+" 1:"+String(data[1])+" 2:"+String(data[2])+" 3:"+String(data[3]),"I²C TEST",4);

    return true;
}

boolean setFreeMap(boolean in[4]){
    for(int i = 0; i<4;i++){
        freeMap[i] = in[i];
    }
}


boolean getSensorVals(){
    Wire.requestFrom(0x02, 4);
    dist_fwd = Wire.read();
    dist_back = Wire.read();
    dist_left = Wire.read();
    dist_right = Wire.read();
    return false;
}



boolean equals(String a, String b){ //IT IS I ; MEGAMIND; AND I AM TOO LAZY TO FIX MISTKAES
    return a.equals(b);
}

//0 : foced
//1 : error
//2 : warnings
//3 : achivements
//4 : states

void HandleSerialIn(String Message){
    int whyudick = Message.length();
    Message[whyudick-1] = '\0';
    Serial1.println(Message);
    Serial.println(Message);
    Mode = Message;
}
/*
void HandleSerialIn(char[] Message){
    int whyudick = strlen(Message);
    Message[whyudick-1] = '\0';
    Serial.println("USER > "+Message);
    Serial1.println("USER > "+Message);
    String Operator = split(Message," ",0);
    String Val1 = split(Message," ",1);
    String Val2 = split(Message," ",2);
    String Val3 = split(Message," ",3);
    Serial.println(Operator+","+Val1+","+Val2+","+Val3);

    if(Operator.equals("set")){
        if(Val1.equals("DebugLevel")){
            DebugLevel = Val2.toInt();
            DevLog("ok","cnsl_set",4);
        }
        if(Val1.equals("MotorTime")){
            motorTimeMax = Val2.toInt();
            DevLog("ok","cnsl_set",4);
        }
        if(Val1.equals("SensorTime")){
            sensorTimeMax = Val2.toInt();
            DevLog("ok","cnsl_set",4);
        }
        if(Val1.equals("Mode")){
            Mode = Val2;
            DevLog("ok","cnsl_set",4);
            submode = "startup";
        }
    }


    if(Operator.equals("get")){
        if(Val1.equals("DebugLevel")){
            DevLog("DebugLevel is "+String(DebugLevel),"cnsl_get",4);
        }
        if(Val1.equals("MotorTime")){
            DevLog("MotorTime is "+String(motorTimeMax),"cnsl_get",4);
        }
        if(Val1.equals("SensorTime")){
            DevLog("SensorTime is "+String(sensorTimeMax),"cnsl_get",4);
        }
        if(Val1.equals("Mode")){
            DevLog("Mode is "+String(Mode),"cnsl_get",4);
        }
    }
}*/



	
String split(String s, char parser, int index) {        //I STOLE THIS CODE
  String rs="";                                         //I STOLE THIS CODE
  int parserIndex = index;                              //I STOLE THIS CODE
  int parserCnt=0;                                      //I STOLE THIS CODE
  int rFromIndex=0, rToIndex=-1;                        //I STOLE THIS CODE
  while (index >= parserCnt) {                          //I STOLE THIS CODE
    rFromIndex = rToIndex+1;                            //I STOLE THIS CODE
    rToIndex = s.indexOf(parser,rFromIndex);            //I STOLE THIS CODE
    if (index == parserCnt) {                           //I STOLE THIS CODE
      if (rToIndex == 0 || rToIndex == -1) return "";   //I STOLE THIS CODE
      return s.substring(rFromIndex,rToIndex);          //I STOLE THIS CODE
    } else parserCnt++;                                 //I STOLE THIS CODE
  }                                                     //I STOLE THIS CODE
  return rs;                                            //I STOLE THIS CODE
}                                                       //I STOLE THIS CODE



