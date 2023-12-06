#include <Wire.h>
//#include <Adafruit_NeoPixel.h>
byte data[4] = {0,0,0,30};

String submode = "startup";

int inputWDT = 3600000;
//Adafruit_NeoPixel pixels = Adafruit_NeoPixel(4, 12, NEO_GRB + NEO_KHZ800);

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
String Mode = "RAMPAGE";


void DevLog(String Message,String Origin, int level = 1){
    if(level >= DebugLevel){
        const String Codes[] = {"FORCED","ERROR","WARNING","SUCCESS","STATE"};
        String ErrorCode = Codes[level];
        String msg = Origin+": "+ErrorCode+" -> "+Message;
        Serial1.print(msg);  //MotorCalc: ERROR -> NullPointerException
        Serial.print(msg);
    }
}


void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial1.begin(115200);


  loopMarker = millis();
}

void loop() {
    LoopTime = millis()-loopMarker;
    loopMarker = millis();
    
    if(Serial1.available()){
        HandleSerialIn(Serial1.readString());
    }
    if(Serial.available()){
        HandleSerialIn(Serial.readString());
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
            
        }else if(submode.equals("center_forward")){
            
        }else if(submode.equals("center_backwards")){
            
        }else if(submode.equals("mov_left")){
            
        }else if(submode.equals("mov_right")){
            
        }else if(submode.equals("check")){
            #define rotTime 2000
            setFreeMap(FREE);
            if(millis()<start+rotTime){
                motorFWD = 0;
                motorMULT = 10;
                motorROT = -64;
                motorSIDE = 0;
            }else if(millis()<start+(rotTime*3000)){
                motorFWD = 0;
                motorMULT = 10;
                motorROT = 64;
                motorSIDE = 0;
                ezmap();
            }
        }else if(submode.equals("honk")){
            
        }else if(submode.equals("sad")){
            //playingsound("sad");
        }else if(submode.equals("a")){
            
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
    }else if(Mode == "RAMPAGE"){
        motorFWD = 0;
        motorMULT = 0;
        motorROT = 127;
        motorSIDE = 40;
    }



    MotorWDT += LoopTime;
    if(MotorWDT > motorTimeMax){
        if(setMotorSpeeds()){
            MotorWDT = 0;
        }
    }
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
    String Operator = "";//split(Message," ",0);
    String Val1 = split(Message," ",1);
    String Val2 = split(Message," ",2);
    String Val3 = split(Message," ",3);

    if(Operator.equals("set")){
        if(Val1.equals("DebugLevel")){
            DebugLevel = Val2.toInt();
        }
        if(Val1.equals("MotorTime")){
            motorTimeMax = Val2.toInt();
        }
        if(Val1.equals("SensorTime")){
            sensorTimeMax = Val2.toInt();
        }
        if(Val1.equals("Mode")){
            Mode = Val2;
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
}



	
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



