#include "defMap.cpp"
#include <Wire.h>


void setup(){
    Wire.begin();
    delay(5000);
}

void loop(){
    Wire.beginTransmission(0x01);
    Wire.write(255);
    Wire.write(0);
    Wire.write(0);
    Wire.write(3);
    Wire.endTransmission();
    delay(5000);

    Wire.beginTransmission(0x01);
    Wire.write(0);
    Wire.write(0);
    Wire.write(0);
    Wire.write(0);
    Wire.endTransmission();
    delay(5000);
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