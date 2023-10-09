#include "defMap.cpp"

int FWD[]  = { 1, 1, 1, 1};
int SIDE[] = {-1, 1, -1,1};
int ROT[]  = { 1,-1, -1,1};

//VL VR HR HL

boolean stopped = true;

int wheelSpeeds[] = {0,0,0,0};


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
    delay(5000);

    digitalWrite(MOTOR_ENABLE,false);

}
void loop(){
    manualAll(16000,FWD);
    manualAll(-16000,FWD);
    delay(500);
    manualAll(16000,SIDE);
    manualAll(-16000,SIDE);
    delay(500);
    manualAll(16000,ROT);
    manualAll(-16000,ROT);
    delay(500);

}


void manualAll(int velo, int dirmap[4]){
    wheelSpeeds[0] = velo*dirmap[0];
    wheelSpeeds[1] = velo*dirmap[1];
    wheelSpeeds[2] = velo*dirmap[2];
    wheelSpeeds[3] = velo*dirmap[3];

    for(int i = 0; i < 80;i++){
        stepSteppers(speedRamp(i,6000,20,20,80));
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


void calculateStepSpeeds(int velocity,int angle,int rotationVel){
  float Fangle = map(angle,0,180,-1,1);
  int fwdComp  = cos(Fangle) * velocity;
  int sideComp = sin(Fangle) * velocity;
  int rotComp  = rotationVel;
  stopped = true;
  for(int i = 0; i < 4;i++){
    wheelSpeeds[i] = (FWD[i]*fwdComp) + (SIDE[i]*fwdComp) + (ROT[i]*fwdComp);
    if(wheelSpeeds[i] != 0){
        stopped = false;
    }
  }
}

boolean procStep(int steps,int step){
    int stackA = int(steps+1 / 2);
    int stackB = int(steps / 2);
    if(step >= 29){
        return step%30 < stackA;
    }else{
        return step%30 < stackB;
    }
}


void stepSteppers(int freq){

    int stepsA = wheelSpeeds[0];
    int stepsB = wheelSpeeds[1];                               //absoluten wert aller step zahlen errechen (negative werte sttehen für rückwärts)
    int stepsC = wheelSpeeds[2];
    int stepsD = wheelSpeeds[3];

    int absStepsA = abs(stepsA);
    int absStepsB = abs(stepsB);                               //absoluten wert aller step zahlen errechen (negative werte sttehen für rückwärts)
    int absStepsC = abs(stepsC);
    int absStepsD = abs(stepsD);

    

    digitalWrite(MOTOR_A_DIR,stepsA<0);
    digitalWrite(MOTOR_B_DIR,stepsB<0);                        //für rückwärts fahren den dir pin wechseln
    digitalWrite(MOTOR_C_DIR,stepsC<0);
    digitalWrite(MOTOR_D_DIR,stepsD<0);

    for(int i = 0; i < 60; i++){
        digitalWrite(MOTOR_A_STEP,procStep(absStepsA,56));
        digitalWrite(MOTOR_B_STEP,procStep(absStepsB,59));      //alle benötigten step pins auf HIGH
        digitalWrite(MOTOR_C_STEP,procStep(absStepsC,59));
        digitalWrite(MOTOR_D_STEP,procStep(absStepsD,59));
        delayMicroseconds(500000/freq);                         //1. hälfte von 600Hz repeater
        digitalWrite(MOTOR_A_STEP,false);
        digitalWrite(MOTOR_B_STEP,false);
        digitalWrite(MOTOR_C_STEP,false);                      //Alle step pins ausschalten
        digitalWrite(MOTOR_D_STEP,false);
        delayMicroseconds(500000/freq);                         //2. hälfte von 600Hz repeater
    }
}
