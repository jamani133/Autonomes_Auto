//4x  3p lidar
#define IRD_A A0 
#define IRD_B A1
#define IRD_C A2
#define IRD_D A3
//4x  4p sonic
#define SD_A_TRIG 6
#define SD_A_ECHO 5

#define SD_B_TRIG 8
#define SD_B_ECHO 7

#define SD_C_TRIG 10 
#define SD_C_ECHO 9     //ALLE HEADER HABEN EINEN VCC UND EINEN GND PIN

#define SD_D_TRIG 12
#define SD_D_ECHO 11

//1x5p driverr
//scl
//sda
#define VOLTAGE_DIV A6

//1x4p uart
//TX
//RX

//2x switch
#define SW_A 4
#define SW_B 3


//1x3p led
#define LED 13
#define LED_NUM 20

#define AUDIO_MOSI 2
#define AUDIO_MISO 21

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