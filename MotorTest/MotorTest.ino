#include "defMap.hpp"
//#include "speedRamp.ino"

int FWD[]  = { 10, 10, 10, 10};

int EXP[]  = { 3, 1, 7, 10};

int SIDE[] = {-10, 10, -10,10};
int ROT[]  = { 10,-10, -10,10};



//VL VR HR HL

int wheelFreq[] = {0,0,0,0};

//int curSteps[] = {0,0,0,0};



void setup(){
    
    configurePins();
    digitalWrite(MOTOR_ENABLE,true);
    delay(5000);

    digitalWrite(MOTOR_ENABLE,false);

}
void loop(){
    const int dur = 5000;
    const int velo = 200; 
    digitalWrite(MOTOR_ENABLE,false);
    manualAll(velo,EXP,dur);
    manualAll(-velo,EXP,dur);
    digitalWrite(MOTOR_ENABLE,true);
    delay(3000);
    digitalWrite(MOTOR_ENABLE,false);
    manualAll(velo,FWD,dur);
    manualAll(-velo,FWD,dur);
    digitalWrite(MOTOR_ENABLE,true);
    delay(3000);
    digitalWrite(MOTOR_ENABLE,false);
    manualAll(velo,SIDE,dur);
    manualAll(-velo,SIDE,dur);
    digitalWrite(MOTOR_ENABLE,true);
    delay(3000);
    digitalWrite(MOTOR_ENABLE,false);
    manualAll(velo,ROT,dur);
    manualAll(-velo,ROT,dur);
    digitalWrite(MOTOR_ENABLE,true);
    delay(3000);

}


void manualAll(int velo, int dirmap[4],int time){
    int times[] = {0,0,0,0};
    //curSteps = {0,0,0,0};
    for(int i = 0; i < 4; i++){
        wheelFreq[i] = velo*dirmap[i];
        digitalWrite(MOTOR_DIR[i],wheelFreq[i]<0);
        times[i] = 1000000/abs(wheelFreq[i]);
    }
    int start = millis();
    while(start+time > millis()){
        runSteppers(times);
    }

    
}

void runSteppers(int times[]){
    for(int i = 0; i < 4; i++){
        digitalWrite(MOTOR_STEP[i],micros() % times[i] > times[i]>>1);
    }
}