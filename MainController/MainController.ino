#include "defMap.cpp"
#include <SharpIR.h>

SharpIR IRD_A = SharpIR(SharpIR::GP2Y0A21YK0F,IRD_A_IN);
SharpIR IRD_B = SharpIR(SharpIR::GP2Y0A21YK0F,IRD_B_IN);
SharpIR IRD_C = SharpIR(SharpIR::GP2Y0A21YK0F,IRD_C_IN);
SharpIR IRD_D = SharpIR(SharpIR::GP2Y0A21YK0F,IRD_D_IN);

void setup(){
    configurePins();
    Serial.begin(115200);
}

void loop(){
    int IRD_A_ACC = 0;
    int IRD_B_ACC = 0;
    int IRD_C_ACC = 0;
    int IRD_D_ACC = 0;
    for(int i = 0; i < 100 ; i++){
        IRD_A_ACC += IRD_A.getDistance();
        IRD_B_ACC += IRD_B.getDistance();
        IRD_C_ACC += IRD_C.getDistance();
        IRD_D_ACC += IRD_D.getDistance();
    }

    Serial.print("IRD_A:");
    Serial.print(IRD_A_ACC/100);
    Serial.print("  IRD_B:");
    Serial.print(IRD_B_ACC/100);
    Serial.print("  IRD_C:");
    Serial.print(IRD_C_ACC/100);
    Serial.print("  IRD_D:");
    Serial.println(IRD_D_ACC/100);
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