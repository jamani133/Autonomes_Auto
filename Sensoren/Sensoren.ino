#include "defMap.cpp" //Variablen
#include <Wire.h> //i²c library
#include <SharpIR.h> //SharpIR library

//Lidar Sensoren 
SharpIR IRD_A = SharpIR(SharpIR::GP2Y0A21YK0F,IRD_A_IN);
SharpIR IRD_B = SharpIR(SharpIR::GP2Y0A21YK0F,IRD_B_IN);
SharpIR IRD_C = SharpIR(SharpIR::GP2Y0A21YK0F,IRD_C_IN);
SharpIR IRD_D = SharpIR(SharpIR::GP2Y0A21YK0F,IRD_D_IN);

//Variablen 2 
unsigned long time;
unsigned long time1;
unsigned long time2;
unsigned long time3;
unsigned long time4;
long cm;

int a; 

int distance1;
int distance2;
int distance3;
int distance4;

long dauer1=0;
long dauer2=0;
long dauer3=0;
long dauer4=0;

long entfernung1=0;
long entfernung2=0;
long entfernung3=0;
long entfernung4=0;

void setup(){
	Serial1.begin (115200);
	configurePins();
	Wire.begin(0x02);
	Wire.onRequest(requestEvent);		
}

void loop(){
  digitalWrite(SD_A_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(SD_A_TRIG, HIGH);          // Senden vom Pulse
  delayMicroseconds(10);
  digitalWrite(SD_A_TRIG, LOW);
  dauer1 = pulseIn(SD_A_ECHO, HIGH);          // Empfang des US-Pulse
  entfernung1 = (dauer1 / 2) / 29;			// Enfernungsberechnung

  digitalWrite(SD_B_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(SD_B_TRIG, HIGH);          // Senden des Pulse
  delayMicroseconds(10);
  digitalWrite(SD_B_TRIG, LOW);
  dauer2 = pulseIn(SD_B_ECHO, HIGH);          // Empfang des US-Pulse
  entfernung2 = (dauer2 / 2) / 29;			// Enfernungsberechnung

  digitalWrite(SD_C_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(SD_C_TRIG, HIGH);          // Senden des Pulse
  delayMicroseconds(10);
  digitalWrite(SD_C_TRIG, LOW);
  dauer3 = pulseIn(SD_C_ECHO, HIGH);          // Empfang des US-Pulse
  entfernung3 = (dauer3 / 2) / 29;			// Enfernungsberechnung

  digitalWrite(SD_D_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(SD_D_TRIG, HIGH);          // Senden des Pulse
  delayMicroseconds(10);
  digitalWrite(SD_D_TRIG, LOW);
  dauer4 = pulseIn(SD_D_ECHO, HIGH);          // Empfang des US-Pulse
  entfernung4 = (dauer4 / 2) / 29;			// Enfernungsberechnung

  distance1 = IRD_A.getDistance();
  distance2 = IRD_B.getDistance();
  distance3 = IRD_C.getDistance();
  distance4 = IRD_D.getDistance();

 Serial1.print("UA:");
 Serial1.print(entfernung1);
 Serial1.print(",UB:"); 
 Serial1.print(entfernung2);
 Serial1.print(",UC:");
 Serial1.print(entfernung3);
 Serial1.print(",UD:");
 Serial1.print(entfernung4);
 Serial1.print(",IA:"); 
 Serial1.print(distance1);
 Serial1.print(",IB:"); 
 Serial1.print(distance2);
 Serial1.print(",IC:"); 
 Serial1.print(distance3);
 Serial1.print(",ID:"); 
 Serial1.println(distance4);
}


void requestEvent(){		//Read = anforderung vom Main
	byte c = Wire.read();
	switch(c){		//sended je nach Anfrage die Entfernung
		case 1:
			Wire.write(entfernung1);
		break;
		case 2:
			Wire.write(entfernung2);
		break;
		case 3:
			Wire.write(entfernung3);
		break;
		case 4:
			Wire.write(entfernung4);
		break;		
		case 5:
			Wire.write(distance1);
		break;
		case 6:
			Wire.write(distance2);
		break;
		case 7:
			Wire.write(distance3);
		break;
		case 8:
			Wire.write(distance4);
		break;
	}
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
