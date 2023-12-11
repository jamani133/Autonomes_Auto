#include "defMap.hpp"  // anderer pin shit idkkkkkk
//#include "speedRamp.ino"
#include "Wire.h"  //i2c shit

int FWD[]  = { -1, -1, -1,-1};
int SIDE[] = { 1,-1,-1,1}; //what even is this
int ROT[]  = { -1,1, -1, -1};
//VL VR HR HL

int wheelFreq[] = {0,0,0,0};  //rad schnell zahl dngs keine ahnung ic hhab zu viel monster energy getrunken ich kann micht nicht dran erinnern
int times[] = {2000,2000,2000,2000};      //anderes red scchnell ding?????????????
//int curSteps[] = {0,0,0,0};
bool running = false;         //is an oder nicht


void setup(){
    Serial1.begin(115200);     //serial begin halt nh
    Wire.begin(0x01);         //i2c anfang dings
    configurePins();          //pin mode oder so
    delay(10000);                     //warum.
    digitalWrite(MOTOR_ENABLE,false);  //motor aus oder so
    Serial1.println("aaaaaaaaaa");     //aaaaaaaaaaaaaaaaaaa
    Wire.onReceive(onWire);           //iwi so das i2c event manager teil
    delay(1000);
}

//mm/1ksteps : 63.81171875
void loop(){
    //Serial.println("weeeeeeee");   //weeeeee
    if(running){      
        runSteppers(times);          //mototor drehen tun
    }else{
        delay(1);                    //mototor nicht drehen tun
    }
}



void runSteppers(int times[]){      //motr dreht sachen
    for(int i = 0; i < 4; i++){ 
        digitalWrite(MOTOR_STEP[i],micros() % long(times[i]) > long(times[i]>>1));  
    } // was in gottes nahmen ist dieser haufen bullshit was hab ih mir hierbei gedacht, egal es funktioniert, don't fix what aint broken
}
//IM GOING INSANE AYEEAAHHHHHH


void onWire(int num){
    //int bFWD = Wire.read();
    //int bSIDE = Wire.read();
    //int bROT = Wire.read();//ok
    //int bMULT = Wire.read();
    int bFWD;
    int bSIDE;
    int bROT; //tHis VaRiaBle waS nOt DecLareD iN ThiS ScOpE   NOW IT IS BITCH!
    int bMULT;
    int Boobies = 0;
    while(Wire.available()){
        switch(Boobies){
            case 0:
                bFWD = Wire.read();  //WHAT
            break;
            case 1:
                bSIDE = Wire.read(); //THE
            break;
            case 2:
                bROT = Wire.read(); //FUCK
            break;
            case 3: 
                bMULT = Wire.read()*16; //IS
            break;
        }
        if(Boobies > 3){Wire.read();}  //THIS????
        Boobies++; //hehe boobies
    }

    //calc freq
    //bMULT = bMULT-127;
    bFWD = bFWD-127;
    bROT = bROT-127; //ashhhhhhhshaiduhasolfzgsdkufhzhzbeswoufbsof8zsgefkuszebf
    bSIDE = bSIDE-127;
    Serial1.println("-----------------------------------");
    Serial1.println(bFWD);
    Serial1.println(bSIDE); // okayyyyyy
    Serial1.println(bROT);
    Serial1.println(bMULT);

    running = false;
    for(int i = 0; i < 4; i++){
        wheelFreq[i] = FWD[i] * bFWD * bMULT;
        wheelFreq[i] += SIDE[i] * bSIDE * bMULT;  // ja. 
        wheelFreq[i] += ROT[i] * bROT * bMULT;
        if(wheelFreq[i] != 0){
            running = true;
            times[i] = 1000000/abs(wheelFreq[i]);   //ehhhhh, das tiel macht so dinge und die dange machen dann sachen, die sachen brauchen wir für zeug
        }else{
            times[i] = -1;
        }
        digitalWrite(MOTOR_DIR[i],wheelFreq[i]<0); //was?
        
    }
    digitalWrite(MOTOR_ENABLE,!running);          //motor aus glaube ich
}