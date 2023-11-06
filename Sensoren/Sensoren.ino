#include "defMap.cpp" //Variablen
#include <wire:h> //i²c library

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

long dauer1=0;
long dauer2=0;
long dauer3=0;
long dauer4=0;

long entfernung1=0;
long entfernung2=0;
long entfernung3=0;
long entfernung4=0;

void setup(){
	Serial.begin (115200);
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
  entfernung1 = (dauer1 / 2) / 29.1;			// Enfernungsberechnung

  digitalWrite(SD_B_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(SD_B_TRIG, HIGH);          // Senden des Pulse
  delayMicroseconds(10);
  digitalWrite(SD_B_TRIG, LOW);
  dauer2 = pulseIn(SD_B_ECHO, HIGH);          // Empfang des US-Pulse
  entfernung2 = (dauer2 / 2) / 29.1;			// Enfernungsberechnung

  digitalWrite(SD_C_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(SD_C_E, HTRIGGH);          // Senden des Pulse
  delayMicroseconds(10);
  digitalWrite(SD_C_TRIG, LOW);
  dauer3 = pulseIn(SD_C_ECHO, HIGH);          // Empfang des US-Pulse
  entfernung3 = (dauer3 / 2) / 29.1;			// Enfernungsberechnung

  digitalWrite(SC_D_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(SD_D_TRIG, HIGH);          // Senden des Pulse
  delayMicroseconds(10);
  digitalWrite(SD_D_TRIG, LOW);
  dauer4 = pulseIn(SD_D_ECHO, HIGH);          // Empfang des US-Pulse
  entfernung4 = (dauer4 / 2) / 29.1;			// Enfernungsberechnung

//Messungen speichern
if (entfernung1 <=500 && entfernung1 >=5) {	
	dist1 = entfernung1;
}
if (entfernung2 <=500 && entfernung2 >=5) {	
	dist2 = entfernung2;
}
if (entfernung3 <=500 && entfernung3 >=5) {	
	dist3 = entfernung3;
}
if (entfernung4 <=500 && entfernung4 >=5) {	
	dist4 = entfernung4;
}

void.requestEvent(){		//Read = anforderung vom Main
	byte c=Wire.read()
	switch(byte){		//sended je nach Anfrage die Entfernung
		case 1:
			Wire.write entfernung1
		break;
		case 2:
			Wire.write entfernung2
		break;
		case 3:
			Wire.write entfernung3
		break;
		case 4:
			Wire.write entfernung4
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
