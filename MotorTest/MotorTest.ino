#include "defMap.cpp"

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


    digitalWrite(MOTOR_ENABLE,false);
}
void loop(){
for(int i = 0; i < 3000;i++){
    digitalWrite(MOTOR_A_STEP,true);
    delay(1);
    digitalwrite(MOTOR_A_STEP,false);
    delay(1);
}
for(int i = 0; i < 3000;i++){
    digitalWrite(MOTOR_B_STEP,true);
    delay(1);
    digitalwrite(MOTOR_B_STEP,false);
    delay(1);
}
for(int i = 0; i < 3000;i++){
    digitalWrite(MOTOR_C_STEP,true);
    delay(1);
    digitalwrite(MOTOR_C_STEP,false);
    delay(1);
}
for(int i = 0; i < 3000;i++){
    digitalWrite(MOTOR_D_STEP,true);
    delay(1);
    digitalwrite(MOTOR_D_STEP,false);
    delay(1);
}
}
