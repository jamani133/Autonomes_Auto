#include "defMap.cpp"

int FWD[]  = { 10, 10, 10, 10};

int EXP[]  = { 3, 1, 7, 10};

int SIDE[] = {-10, 10, -10,10};
int ROT[]  = { 10,-10, -10,10};

//VL VR HR HL

boolean stopped = true;
int wheelSpeeds[] = {0,0,0,0};
int wheelAbs[] = {0,0,0,0};
int lastStep[] = {0,0,0,0};

void configurePins(){
    pinMode(MOTOR_A_STEP,OUTPUT);
    pinMode(MOTOR_A_DIR, OUTPUT);
    pinMode(MOTOR_B_STEP,OUTPUT);
    pinMode(MOTOR_B_DIR, OUTPUT);
    pinMode(MOTOR_C_STEP,OUTPUT);
    pinMode(MOTOR_C_DIR, OUTPUT);
    pinMode(MOTOR_D_STEP,OUTPUT);
    pinMode(MOTOR_D_DIR, OUTPUT);
    pinMode(MOTOR_ENABLE,OUTPUT);
}


void setup(){
    
    configurePins();
    digitalWrite(MOTOR_ENABLE,true);
    delay(5000);

    digitalWrite(MOTOR_ENABLE,false);

}
void loop(){
    digitalWrite(MOTOR_ENABLE,false);
    manualAll(2,EXP,3000);
    manualAll(-2,EXP,3000);
    digitalWrite(MOTOR_ENABLE,true);
    digitalWrite(MOTOR_ENABLE,false);
    manualAll(2,FWD,3000);
    manualAll(-2,FWD,3000);
    digitalWrite(MOTOR_ENABLE,true);
    delay(3000);
    digitalWrite(MOTOR_ENABLE,false);
    manualAll(2,SIDE,3000);
    manualAll(-2,SIDE,3000);
    digitalWrite(MOTOR_ENABLE,true);
    delay(3000);
    digitalWrite(MOTOR_ENABLE,false);
    manualAll(2,ROT,3000);
    manualAll(-2,ROT,3000);
    digitalWrite(MOTOR_ENABLE,true);
    delay(3000);

}


void manualAll(int velo, int dirmap[4],int dist){

    for(int i = 0; i < 4; i++){
        wheelSpeeds[i] = velo*dirmap[i]*15;
        wheelAbs[i] = abs(wheelSpeeds[i]);
    }
    digitalWrite(MOTOR_A_DIR,wheelSpeeds[0]<0);
    digitalWrite(MOTOR_B_DIR,wheelSpeeds[1]<0);                        //für rückwärts fahren den dir pin wechseln
    digitalWrite(MOTOR_C_DIR,wheelSpeeds[2]<0);
    digitalWrite(MOTOR_D_DIR,wheelSpeeds[3]<0);
    int start = millis();
    while(start+dist > millis()){
        stepSteppers();  //15st/s/mm/s   speedRamp(millis()-start,velo*15,20,20,dist)
    }
}

int speedRamp(int cur, int maxSpeed, int rampup, int rampdown, int duration){
    if(cur >= rampup){
        if(cur>=duration-rampdown){
            return(int(map(cur,duration-rampdown,duration,maxSpeed,0))); //down
        }else{
            return(maxSpeed); //hold
        }
    }else{
        //up
        return(int(map(cur,0,rampup,0,maxSpeed)));
    }
}


boolean procStep(int freq, int i){
    int time = 1000000/freq;
    if(micros() > lastStep[i]+time){
        lastStep[i] = micros();
        return true;
    }
    return false;
}


void stepSteppers(){
    digitalWrite(MOTOR_A_STEP,procStep(wheelAbs[0],0));
    digitalWrite(MOTOR_B_STEP,procStep(wheelAbs[1],1));      //alle benötigten step pins auf HIGH
    digitalWrite(MOTOR_C_STEP,procStep(wheelAbs[2],2));
    digitalWrite(MOTOR_D_STEP,procStep(wheelAbs[3],3));
}
