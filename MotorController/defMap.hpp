
//drivers
#define MOTOR_ENABLE 12

const int MOTOR_STEP[] = {10,9,6,2};
const int MOTOR_DIR[] = {11,8,7,4};



void configurePins(){
    for(int i = 0; i < 4; i++){
        pinMode(MOTOR_STEP[i],OUTPUT);
        pinMode(MOTOR_DIR[i], OUTPUT);
    }
}
//1x5p interconnet
//GND
//VCC
//VBAT
//SCL
//SDA

//everey motordriver :
//rst - > sleep
//en to en rail
//M1 low
//m2 low
//M3 low
//VMOT -> VBAT
//VCC -> 5V
//GND -> >GND
