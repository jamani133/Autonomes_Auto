#include "defMap.cpp"
#include <Wire.h>

SharpIR IRD_A = SharpIR(SharpIR::GP2Y0A21YK0F,IRD_A_IN);
SharpIR IRD_B = SharpIR(SharpIR::GP2Y0A21YK0F,IRD_B_IN);
SharpIR IRD_C = SharpIR(SharpIR::GP2Y0A21YK0F,IRD_C_IN);
SharpIR IRD_D = SharpIR(SharpIR::GP2Y0A21YK0F,IRD_D_IN);

unsigned long time;
unsigned long time1;
unsigned long time2;
unsigned long time3;
unsigned long time4;
long cm;

int a; 

void setup(){
    Wire.begin();
    delay(5000);
}

void loop(){
   
}



void configurePins(){
    pinMode(SD_A_TRIG,OUTPUT);
    pinMode(SD_A_ECHO,INPUT);
    pinMode(SD_B_TRIG,OUTPUT);
    pinMode(SD_B_ECHO,INPUT);
    pinMode(SD_C_TRIG,OUTPUT);
    pinMode(SD_C_ECHO,INPUT);
    pinMode(SD_D_TRIG,OUTPUT);
    pinMode(SD_D_ECHO,INPUT);

    pinMode(SW_A,INPUT_PULLUP);
    pinMode(SW_B,INPUT_PULLUP);
}