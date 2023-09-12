
//drivers
#define MOTOR_ENABLE 12

#define MOTOR_A_STEP 11
#define MOTOR_A_DIR 10

#define MOTOR_A_STEP 9
#define MOTOR_A_DIR 8

#define MOTOR_A_STEP 7
#define MOTOR_A_DIR 6

#define MOTOR_A_STEP 5
#define MOTOR_A_DIR 4

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

void configurePins(){
    pinMode(MOTOR_A_STEP,OUTPUT);
    pinMode(MOTOR_A_DIR,OUTPUT);
    pinMode(MOTOR_B_STEP,OUTPUT);
    pinMode(MOTOR_B_DIR,OUTPUT);
    pinMode(MOTOR_C_STEP,OUTPUT);
    pinMode(MOTOR_C_DIR,OUTPUT);
    pinMode(MOTOR_D_STEP,OUTPUT);
    pinMode(MOTOR_D_DIR,OUTPUT);

    pinMode(MOTOR_ENABLE,OUTPUT);
}