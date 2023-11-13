#include <Wire.h>
byte data[4] = {0,0,0,30};


int SensorWDT = 0;
int MotorWDT = 0;
int TelemWDT = 0;

int LoopTime = 0;
long loopMarker = 0;


int DebugLevel = 4; //    -1 - 4 
String Mode = "IDLE";

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial1.begin(115200);


  loopMarker = millis();
}

void loop() {
    loopTime = millis()-loopMarker;
    loopMarker = millis();


    SensorWDT += loopTime;
    if(SensorWDT > 300){
        if(getSensorVals()){
            SensorWDT = 0;
        }
    }

    MotorWDT += loopTime;
    if(MotorWDT > 300){
        if(setMotorSpeeds()){
            MotorWDT = 0;
        }
    }
}


Boolean setMotorSpeeds(){
    data[0] = 1;
    data[1] = 2;
    data[2] = 3;
    Wire.beginTransmission(0x01);
    Wire.write(data,4);
    Wire.endTransmission();
    Serial.println("sending...");
    Serial1.println();
    DevLog("0:"+String(data[0])+" 1:"+String(data[1])+" 2:"+String(data[2])+" 3:"+String(data[3]),"I²C TEST",4);

    return true;
}



Boolean getSensorVals(){
    Wire.requestFrom(0x02, 1);
	while(Wire.available()){
		char c=Wire.read();
    }
    return false;
}

//0 : foced
//1 : error
//2 : warnings
//3 : achivements
//4 : states

void HandleSerialIn(String Message){
    String operator = split(Message," ",0);
    String Val1 = split(Message," ",1);
    String Val2 = split(Message," ",2);
    String Val3 = split(Message," ",3);

    if(equals(operator,"set")){
        if(equals(Val1,"DebugLevel")){
            DebugLevel = Val2.toInt();
        }
        if(equals(Val1,"Mode")){
            Mode = Val2;
        }
    }


    if(equals(operator,"get")){
        if(equals(Val1,"DebugLevel")){
            DevLog("DebugLevel is "+String(DebugLevel),"cnsl_get",0);
        }
        if(equals(Val1,"Mode")){
            DevLog("Mode is "+String(Mode),"cnsl_get",0);
        }
    }
}

void DevLog(String Message,String Origin, int level = 1){
    if(level >= DebugLevel){
        const String Codes[] = {"FORCED","ERROR","WARNING","SUCCESS","STATE"};
        String ErrorCode = Codes[level];
        String msg = Origin+": "+ErrorCode+" -> "+Message;
        Serial1.print(msg);  //MotorCalc: ERROR -> NullPointerException
        Serial.print(msg);
    }
}

	
String split(String s, char parser, int index) { //I STOLE THIS CODE
  String rs="";
  int parserIndex = index;
  int parserCnt=0;
  int rFromIndex=0, rToIndex=-1;
  while (index >= parserCnt) {
    rFromIndex = rToIndex+1;
    rToIndex = s.indexOf(parser,rFromIndex);
    if (index == parserCnt) {
      if (rToIndex == 0 || rToIndex == -1) return "";
      return s.substring(rFromIndex,rToIndex);
    } else parserCnt++;
  }
  return rs;
}



