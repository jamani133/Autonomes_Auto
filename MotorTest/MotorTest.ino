#include "defMap.hpp"
//#include "speedRamp.ino"
#include "Wire.h"

int FWD[]  = { 1, 1, 1, 1};
int SIDE[] = {-1, 1, 1,-1};
int ROT[]  = { 1,-1, 1,-1};
//VL VR HR HL

int wheelFreq[] = {0,0,0,0};
int times[] = {0,0,0,0};
//int curSteps[] = {0,0,0,0};
bool running = false;


void setup(){
    Serial.begin(115200);
    Wire.begin(0x01);
    configurePins();
    delay(10000);
    digitalWrite(MOTOR_ENABLE,true);
    Serial.println("aaaaaaaaaa");
    Wire.onReceive(onWire);
}

//mm/1ksteps : 63.81171875
void loop(){
    //Serial.println("weeeeeeee");
    if(running){
        runSteppers(times);
    }else{
        delay(1);
    }
}



void runSteppers(int times[]){
    for(int i = 0; i < 4; i++){
        digitalWrite(MOTOR_STEP[i],micros() % long(times[i]) > long(times[i]>>1));
    }
}


void onWire(int num){
    //int bFWD = Wire.read();
    //int bSIDE = Wire.read();
    //int bROT = Wire.read();
    //int bMULT = Wire.read();

    int bFWD = Wire.read();
    int bSIDE = Wire.read();
    int bROT = Wire.read();
    int bMULT = Wire.read();
    //calc freq
    bMULT = bMULT-127;
    Serial.print("-----------------------------------");
    Serial.println(bFWD);
    Serial.println(bSIDE);
    Serial.println(bROT);
    Serial.println(bMULT);

    running = false;
    for(int i = 0; i < 4; i++){
        wheelFreq[i] = FWD[i] * bFWD * bMULT;
        wheelFreq[i] = SIDE[i] * bSIDE * bMULT;
        wheelFreq[i] = ROT[i] * bROT * bMULT;
        if(wheelFreq[i] != 0){
            running = true;
            times[i] = 1000000/abs(wheelFreq[i]);
        }else{
            times[i] = -1;
        }
        digitalWrite(MOTOR_DIR[i],wheelFreq[i]<0);
        
    }
    digitalWrite(MOTOR_ENABLE,!running);
}