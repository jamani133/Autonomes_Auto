#include "defMap.cpp" //andere datei dings für pinbelegung
#include <Wire.h> //I²C lib


// A    B
                 //welcher motor wo ist

// C    D

int FWD[]  = { 1, 1, 1, 1};
int SIDE[] = {-1, 1, 1,-1};       //komische mathe kacke die ich nd beschreiben kann aber die werte werden mit der geschwindigkeit multipliziert dait die motoren sic hbewegen
int ROT[]  = { 1,-1, 1,-1};

boolean stopped = true;  //var für ob motor an oder nicht

int wheelSpeeds[] = {0,0,0,0}; //ic hhab vergessen was da s hier macht

void configurePins(){
    pinMode(MOTOR_A_STEP,OUTPUT);
    pinMode(MOTOR_A_DIR, OUTPUT);
    pinMode(MOTOR_B_STEP,OUTPUT);
    pinMode(MOTOR_B_DIR, OUTPUT);
    pinMode(MOTOR_C_STEP,OUTPUT);
    pinMode(MOTOR_C_DIR, OUTPUT); //mach da pinmode kacke dingsda
    pinMode(MOTOR_D_STEP,OUTPUT);
    pinMode(MOTOR_D_DIR, OUTPUT);
    pinMode(MOTOR_ENABLE,OUTPUT);
}


void setup(){
    
    configurePins(); //pin modes setzen    siehe oben
    Wire.begin(2);   //i²c dings initialisieren

    calculateStepSpeeds(30,0,0);         //???????
    digitalWrite(MOTOR_ENABLE,stopped);  //motor aus machen
}
void loop(){

    if(Wire.available() >= 3){  //wenn da si²c is dann i²c auslesen
        byte[3] penis = Wire.read();
        int vel = penis[0];          //ICH KANN NICHT MEHRRRRRRRRRRRRR
        int ang = penis[1];
        int rot = penis[2];
        calculateStepSpeeds(vel,ang,rot);      //richtung und so ausrechenn
        digitalWrite(MOTOR_ENABLE,stopped);    //motor aus machen
    }
    if(!stopped){
        stepSteppers();   //motror drehen yeeee
    }
    
    //5V
    //GND
    //SDA
    //SCL
    //VBat
    
}

void calculateStepSpeeds(int velocity,int angle,int rotationVel){
  float Fangle = map(angle,0,255,-1,1);
  int fwdComp  = cos(Fangle) * velocity;          //WHAT THE FUC k IS >THIS
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
