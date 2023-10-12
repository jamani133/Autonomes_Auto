#include "defMap.hpp"
#include "Bresenham.hpp"
//#include "speedRamp.ino"

int FWD[]  = { 10, 10, 10, 10};

int EXP[]  = { 3, 1, 7, 10};

int SIDE[] = {-10, 10, -10,10};
int ROT[]  = { 10,-10, -10,10};


Bresenham Bres[4];


//VL VR HR HL

boolean stopped = true;
int wheelDist[] = {0,0,0,0};
int wheelAbs[] = {0,0,0,0};




void setup(){
    
    configurePins();
    digitalWrite(MOTOR_ENABLE,true);
    delay(5000);

    digitalWrite(MOTOR_ENABLE,false);

}
void loop(){
    digitalWrite(MOTOR_ENABLE,false);
    manualAll(1000,EXP,300);
    manualAll(1000,EXP,-300);
    digitalWrite(MOTOR_ENABLE,true);
    delay(3000);
    digitalWrite(MOTOR_ENABLE,false);
    manualAll(1000,FWD,300);
    manualAll(1000,FWD,-300);
    digitalWrite(MOTOR_ENABLE,true);
    delay(3000);
    digitalWrite(MOTOR_ENABLE,false);
    manualAll(1000,SIDE,300);
    manualAll(1000,SIDE,-300);
    digitalWrite(MOTOR_ENABLE,true);
    delay(3000);
    digitalWrite(MOTOR_ENABLE,false);
    manualAll(1000,ROT,300);
    manualAll(1000,ROT,-300);
    digitalWrite(MOTOR_ENABLE,true);
    delay(3000);

}


void manualAll(int velo, int dirmap[4],int dist){

    for(int i = 0; i < 4; i++){
        wheelDist[i] = dist*dirmap[i];
        wheelAbs[i] = abs(wheelDist[i]);
    }

    stepSteppers(velo*1000);  //15st/s/mm/s   speedRamp(millis()-start,velo*15,20,20,dist)
    
}


void stepSteppers(int timeFrame){
    for(int i = 0; i < 4; i++){
        digitalWrite(MOTOR_DIR[i], wheelDist[i]<0);
        Bres[i].config(wheelAbs[i],timeFrame);
    }
    int startVal = micros();
    int prevTime = micros();
    while(micros() < startVal+timeFrame){
        for(int i = 0; i < 4; i ++){
            Bres[i].advance(micros()-prevTime);
            digitalWrite(MOTOR_STEP[i],Bres[i].queueStep);
            Bres[i].queueStep = false;
        }
        prevTime = micros();
    }
}
