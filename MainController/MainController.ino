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
long nextDistLog = 0;
int eurobeatCountdown = -1;  //DONT ASK
String allowed[6] = {"","","","","",""};
int sensorTimeMax = 100;
int  motorTimeMax = 100; 
String prevSM = "a";
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
        getSensorVals();
        SensorWDT = 0;
        
    }
    if(millis() > nextDistLog){
        Serial1.print("M:"+Mode+"#");
        Serial1.print("DF:"+String(dist_fwd)+"#");
        Serial1.print("DB:"+String(dist_back)+"#");
        Serial1.print("DL:"+String(dist_left)+"#");
        Serial1.print("DR:"+String(dist_right)+"#");
        nextDistLog = millis() + 1000;
    }
    motorMULT = 16;
    if(Mode == "IDLE"){
        motorFWD = 0;
        motorROT = 0;
        motorSIDE = 0;
        pixels.setPixelColor(0,pixels.Color(255,255,255));
           pixels.setPixelColor(1,pixels.Color(255,255,255));
           pixels.setPixelColor(2,pixels.Color(255,0,0));
           pixels.setPixelColor(3,pixels.Color(255,0,0));
    }else if(Mode == "AUTO"){
        motorMULT = 255;
        if(!submode.equals(prevSM)){
            Serial1.print("SM:"+submode+"#");
        }
        prevSM = submode;
        if(submode.equals("startup")){
            motorFWD = 0;
            motorROT = 0;
            motorSIDE = 0;
                     //fwd, back,left,right
            setFreeMap(FREE);
            if(dist_back <= 30){
        freeMap[1] = false;
        //Serial1.print("LR:back#");
    }
    if(dist_fwd <= 30){
        freeMap[0] = false;
        //Serial1.print("LR:fwd#");
    }
    if(dist_left <= 30){
        freeMap[2] = false;
        //Serial1.print("LR:left#");
    }
    if(dist_right <= 30){
        freeMap[3] = false;
        //Serial1.print("LR:right#");
    }
            if(freeMap == SAD){
                submode = "sad";
            }else{
                submode = "check";
                start = millis();
                setFreeMap(FREE);
            }
            delay(100);
        }else if(submode.equals("forward")){
           if(dist_fwd > 20){
                motorFWD = 127;
                motorROT = 0;
                motorSIDE = 0;
           }else{
                motorFWD = 0;
                motorROT = 0;
                motorSIDE = 0;
                submode = "check";
                start = millis();
                setFreeMap(FREE);
           }
           pixels.setPixelColor(0,pixels.Color(255,255,255));
           pixels.setPixelColor(1,pixels.Color(255,255,255));
           pixels.setPixelColor(2,pixels.Color(64,0,0));
           pixels.setPixelColor(3,pixels.Color(64,0,0));
        }else if(submode.equals("backup")){
           if(dist_back > 20){
                motorFWD = -127;
                motorROT = 0;
                motorSIDE = 0;
           }else{
                motorFWD = 0;
                motorROT = 0;
                motorSIDE = 0;
                submode = "check";
                start = millis();
                setFreeMap(FREE);
           }
           pixels.setPixelColor(0,pixels.Color(0,0,0));
           pixels.setPixelColor(1,pixels.Color(0,0,0));
           pixels.setPixelColor(2,pixels.Color(255,0,0));
           pixels.setPixelColor(3,pixels.Color(255,0,0));
        }else if(submode.equals("left")){
            if(dist_left > 20){
                motorFWD = 0;
                motorROT = 0;
                motorSIDE = -127;
           }else{
                motorFWD = 0;
                motorROT = 0;
                motorSIDE = 0;
                submode = "check";
                start = millis();
                setFreeMap(FREE);
           }
        }else if(submode.equals("right")){
            if(dist_fwd > 20){
                motorFWD = 0;
                motorROT = 0;
                motorSIDE = 127;
           }else{
                motorFWD = 0;
                motorROT = 0;
                motorSIDE = 0;
                submode = "check";
                start = millis();
                setFreeMap(FREE);
           }
        }else if(submode.equals("check")){
            #define rotTime 2000
            
            if(millis()<start+rotTime){
                motorFWD = 0;
                motorMULT = 2;
                motorROT = -8;
                motorSIDE = 0;
            }else if(millis()<start+(rotTime*3)){
                motorFWD = 0;
                motorMULT = 2;
                motorROT = 8;
                motorSIDE = 0;
                if(dist_back <= 30){
        freeMap[1] = false;
        //Serial1.print("LR:back#");
    }
    if(dist_fwd <= 30){
        freeMap[0] = false;
        //Serial1.print("LR:fwd#");
    }
    if(dist_left <= 30){
        freeMap[2] = false;
        //Serial1.print("LR:left#");
    }
    if(dist_right <= 30){
        freeMap[3] = false;
        //Serial1.print("LR:right#");
    }
            }else if(millis()<start+(rotTime*4)){
                motorFWD = 0;
                motorMULT = 2;
                motorROT = -8;
                motorSIDE = 0;
                
            }else if(millis()<start+(rotTime*5)){
            submode = "sad";
            Serial1.print("LR:"+String(freeMap[0])+String(freeMap[1])+String(freeMap[2])+String(freeMap[3])+"#");
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
                    
                    allowDir("left_rot");
                    n++;
                }
                if(freeMap[3]){
                    
                    allowDir("right_rot");
                    n++;
                }
                if(!n == 0){
                    
                    submode = allowed[random(n)];
                    start = millis();
                    Serial1.print("LR:"+String(allowed[0])+String(allowed[1])+String(allowed[2])+String(allowed[3])+"#");
                }
                getSensorVals();
            }
            
            }
            pixels.setPixelColor(0,pixels.Color(0,255,0));
           pixels.setPixelColor(1,pixels.Color(0,255,0));
           pixels.setPixelColor(2,pixels.Color(0,255,0));
           pixels.setPixelColor(3,pixels.Color(0,255,0));
        }else if(submode.equals("left_rot")){
            if(start+1500 > millis()){
                motorFWD = 0;
                motorROT = -64;
                motorSIDE = 0;
            }else{
                motorFWD = 0;
                motorROT = 0;
                motorSIDE = 0;
                submode = "forward";
            }
            pixels.setPixelColor(0,pixels.Color(0,0,0));
           pixels.setPixelColor(1,pixels.Color(255,255,0));
           pixels.setPixelColor(2,pixels.Color(255,255,0));
           pixels.setPixelColor(3,pixels.Color(0,0,0));
        }else if(submode.equals("right_rot")){
            if(start+1500 > millis()){
                motorFWD = 0;
                motorROT = 64;
                motorSIDE = 0;
            }else{
                motorFWD = 0;
                motorROT = 0;
                motorSIDE = 0;
                submode = "forward";
            }
            pixels.setPixelColor(0,pixels.Color(255,255,0));
           pixels.setPixelColor(1,pixels.Color(0,0,0));
           pixels.setPixelColor(2,pixels.Color(0,0,0));
           pixels.setPixelColor(3,pixels.Color(255,255,0));
        }else if(submode.equals("honk")){
            //honk
        }else if(submode.equals("sad")){
            //playingsound("sad");
            pixels.setPixelColor(0,pixels.Color(0,0,255));
           pixels.setPixelColor(1,pixels.Color(0,0,255));
           pixels.setPixelColor(2,pixels.Color(0,0,255));
           pixels.setPixelColor(3,pixels.Color(0,0,255));
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
        motorROT = 127; //FIDGET SPINNERRRRRRRR
        motorSIDE = 0;
    }else if(Mode == "RAMPAGE2"){
        motorFWD = 0;
        motorMULT = 127;   //why
        motorROT = 64;
        motorSIDE = 127;
    }else if(Mode == "RAMPAGE3"){
        motorFWD = 0;
        motorMULT = 127;
        motorROT = 90;     //ok
        motorSIDE = 90;
    }else if(Mode == "RAMPAGE4"){
        motorFWD = 90;
        motorMULT = 127;
        motorROT = 90;  //what
        motorSIDE = 0;
    }else if(Mode == "fwd"){
        motorFWD = 127;
        motorMULT = 255;
        motorROT = 0;
        motorSIDE = 0;
        pixels.setPixelColor(0,pixels.Color(255,255,255));
        pixels.setPixelColor(1,pixels.Color(256,255,255));
        pixels.setPixelColor(2,pixels.Color(255,0,0));
        pixels.setPixelColor(3,pixels.Color(255,0,0));
    }else if(Mode == "left"){
        motorFWD = 0;
        motorMULT = 255;
        motorROT = 0;
        motorSIDE = -127;
        pixels.setPixelColor(0,pixels.Color(0,0,0));
        pixels.setPixelColor(1,pixels.Color(255,0,0));
        pixels.setPixelColor(2,pixels.Color(255,0,0));
        pixels.setPixelColor(3,pixels.Color(0,0,0));
    }else if(Mode == "right"){
        motorFWD = 0;
        motorMULT = 255;
        motorROT = 0;
        motorSIDE = 127;
        pixels.setPixelColor(0,pixels.Color(255,0,0));
        pixels.setPixelColor(1,pixels.Color(0,0,0));
        pixels.setPixelColor(2,pixels.Color(0,0,0));
        pixels.setPixelColor(3,pixels.Color(255,0,0));
    }else if(Mode == "back"){
        motorFWD = -127;
        motorMULT = 255;
        motorROT = 0;
        motorSIDE = 0;
        pixels.setPixelColor(0,pixels.Color(0,0,0));
        pixels.setPixelColor(1,pixels.Color(0,0,0));
        pixels.setPixelColor(2,pixels.Color(255,0,0));
        pixels.setPixelColor(3,pixels.Color(255,0,0));
    }else if(Mode == "rleft"){
        motorFWD = 0;
        motorMULT = 255;
        motorROT = -127;
        motorSIDE = 0;
        pixels.setPixelColor(0,pixels.Color(0,0,0));
        pixels.setPixelColor(1,pixels.Color(255,255,0));
        pixels.setPixelColor(2,pixels.Color(0,0,0));
        pixels.setPixelColor(3,pixels.Color(0,0,0));
    }else if(Mode == "rright"){
        motorFWD = 0;
        motorMULT = 255;
        motorROT = 255;
        motorSIDE = 0;
        pixels.setPixelColor(0,pixels.Color(255,255,0));
        pixels.setPixelColor(1,pixels.Color(0,0,0));
        pixels.setPixelColor(2,pixels.Color(0,0,0));
        pixels.setPixelColor(3,pixels.Color(0,0,0));
    }



    MotorWDT += LoopTime;
    if(MotorWDT > motorTimeMax){
        if(setMotorSpeeds()){
            MotorWDT = 0;
        }
    }
    //pixels.setPixelColor(0,pixels.Color(255,255,255));
    //pixels.setPixelColor(1,pixels.Color(255,255,255));
    //pixels.setPixelColor(2,pixels.Color(255,0,0));
    //pixels.setPixelColor(3,pixels.Color(255,0,0));
    pixels.show();
}



boolean setMotorSpeeds(){
    data[0] = motorFWD+127;
    data[1] = motorSIDE+127;  //ye idk
    data[2] = motorROT+127;
    data[3] = motorMULT;
    //Serial1.print("LR:setMotors#");
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
    Serial.println("PINGED FOR SESNSORS YOU DIRTY FUCKING SLUT OF A PIG");
    //while(Wire.available()){
    //    Wire.read();   //flush i2c buffer
    //}
    Wire.requestFrom(2, 4, true);
    //while(Wire.available() == 0){} //wait until recieved, not very gud, and can crahs, but idc
    //Serial1.print("LR:pingedForSensors#");
    
    dist_fwd = Wire.read();
    dist_back = Wire.read();  //read shit value stuff idk
    dist_left = Wire.read();
    dist_right = Wire.read();
    return false; //dummy bc idk
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
    Serial1.println("LR:"+Message+"#");
    Serial.println(Message);
    Mode = Message;   //this is not good, but no language on earth has a word for how little i care, a quantum supercomuter calculating for a thousand years could not even aproach the number of fucks i do not give
    submode = "startup";
    prevSM = "a";
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
/*
int Geschwindigkeit(int distanz, int baseVal){
    #define minDist 10 //cm
    #define maxDist 50 //cm
    #define minSpeed 1 //% von baseVal
    #define maxSpeed 100 //% von bseVal
    int8_t DIR = 1;
    if(baseVal < 0){
        DIR = -1;
    }
    if(baseVal == 0){
        return 0;
    }
    return DIR * constrain(( map(distanz , minDist, maxSpeed, maxSpeed) * baseVal) / 100, minSpeed, maxSpeed);
}*/

