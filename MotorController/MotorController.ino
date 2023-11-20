#include "defMap.cpp"
#include <Wire.h>


// A    B


// C    D

int FWD[]  = { 1, 1, 1, 1};
int SIDE[] = {-1, 1, 1,-1};
int ROT[]  = { 1,-1, 1,-1};

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
    Wire.begin(2);

    calculateStepSpeeds(30,0,0);
    digitalWrite(MOTOR_ENABLE,stopped);
}
void loop(){

    if(Wire.available() >= 3){
        byte[3] =
        int vel = Wire.read();
        int ang = Wire.read();
        int rot = Wire.read();
        calculateStepSpeeds(vel,ang,rot);
        digitalWrite(MOTOR_ENABLE,stopped);
    }
    if(!stopped){
        stepSteppers();
    }
    
    //5V
    //GND
    //SDA
    //SCL
    //VBat
    
}

void calculateStepSpeeds(int velocity,int angle,int rotationVel){
  float Fangle = map(angle,0,255,-1,1);
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


void stepSteppers(){

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
        digitalWrite(MOTOR_A_STEP,procStep(absStepsA,i));
        digitalWrite(MOTOR_B_STEP,procStep(absStepsB,i));      //alle benötigten step pins auf HIGH
        digitalWrite(MOTOR_C_STEP,procStep(absStepsC,i));
        digitalWrite(MOTOR_D_STEP,procStep(absStepsD,i));
        delayMicroseconds(500000/600);                         //1. hälfte von 600Hz repeater
        digitalWrite(MOTOR_A_STEP,false);
        digitalWrite(MOTOR_B_STEP,false);
        digitalWrite(MOTOR_C_STEP,false);                      //Alle step pins ausschalten
        digitalWrite(MOTOR_D_STEP,false);
        delayMicroseconds(500000/600);                         //2. hälfte von 600Hz repeater
    }
}
