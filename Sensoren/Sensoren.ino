#include "defMap.cpp" //Variablen
#include <Wire.h> //i²c libra
//Variablen 2
unsigned long time;
unsigned long time1;
unsigned long time2;
unsigned long time3;
unsigned long time4;
long cm;

int a;


long dauer1 = 0;
long dauer2 = 0;
long dauer3 = 0;
long dauer4 = 0;

int entfernung1 = 0;
int entfernung2 = 0;
int entfernung3 = 0;
int entfernung4 = 0;

int pentfernung1 = 0;
int pentfernung2 = 0;
int pentfernung3 = 0;
int pentfernung4 = 0;


int wdt1 = 0;
int wdt2 = 0;
int wdt3 = 0;
int wdt4 = 0;


int tentfernung1 = 0;
int tentfernung2 = 0;
int tentfernung3 = 0;
int tentfernung4 = 0;


void setup() {
  Serial1.begin (115200);
  configurePins();
  Wire.begin(0x02);
  Wire.onRequest(requestEvent);
}

void loop() {
  digitalWrite(SD_A_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(SD_A_TRIG, HIGH);          // Senden des Pulse
  delayMicroseconds(10);
  digitalWrite(SD_A_TRIG, LOW);
  dauer1 = pulseIn(SD_A_ECHO, HIGH);          // Empfang des US-Pulse
  tentfernung1 = (dauer1 / 2) / 29;      // Enfernungsberechnung
  if (tentfernung1 < 300) {
    if(tentfernung1 > 150){
        tentfernung1 = 150;
    }
    if (wdt1 >= 6 || (tentfernung1 > pentfernung1 - 20 && tentfernung1 < pentfernung1 + 20 )) { //oke
      entfernung1 = (0.5 * entfernung1) + (0.5 * tentfernung1);
      wdt1 = 0;
      pentfernung1 = entfernung1;
      
    } else { //bullshit
      wdt1++;
    }
  }
  delay(25);

  digitalWrite(SD_B_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(SD_B_TRIG, HIGH);          // Senden des Pulse
  delayMicroseconds(10);
  digitalWrite(SD_B_TRIG, LOW);
  dauer2 = pulseIn(SD_B_ECHO, HIGH);          // Empfang des US-Pulse
  tentfernung2 = (dauer2 / 2) / 29;			// Enfernungsberechnung
  if (tentfernung2 < 300) {
    if(tentfernung2 > 150){
        tentfernung2 = 150;
    }
    if (wdt2 >= 6 || (tentfernung2 > pentfernung2 - 20 && tentfernung2 < pentfernung2 + 20 )) { //oke
      entfernung2 = (0.5 * entfernung2) + (0.5 * tentfernung2);
      wdt2 = 0;
      pentfernung2 = entfernung2;
      
    } else { //bullshit
      wdt2++;
    }
  }
  delay(25);
  
  digitalWrite(SD_C_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(SD_C_TRIG, HIGH);          // Senden des Pulse
  delayMicroseconds(10);
  digitalWrite(SD_C_TRIG, LOW);
  dauer3 = pulseIn(SD_C_ECHO, HIGH);          // Empfang des US-Pulse
  tentfernung3 = (dauer3 / 2) / 29;      // Enfernungsberechnung
  if (tentfernung3 < 300) {
    if(tentfernung3 > 150){
        tentfernung3 = 150;
    }
    if (wdt3 >= 6 || (tentfernung3 > pentfernung3 - 20 && tentfernung3 < pentfernung3 + 20 )) { //oke
      entfernung3 = (0.5 * entfernung3) + (0.5 * tentfernung3);
      wdt3 = 0;
      pentfernung3 = entfernung3;
      
    } else { //bullshit
      wdt3++;
    }
  }
  delay(25);

    digitalWrite(SD_D_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(SD_D_TRIG, HIGH);          // Senden des Pulse
  delayMicroseconds(10);
  digitalWrite(SD_D_TRIG, LOW);
  dauer4 = pulseIn(SD_D_ECHO, HIGH);          // Empfang des US-Pulse
  tentfernung4 = (dauer4 / 2) / 29;      // Enfernungsberechnung
  if (tentfernung4 < 300) {
    if(tentfernung4 > 150){
        tentfernung4 = 150;
    }
    if (wdt4 >= 6 || (tentfernung4 > pentfernung4 - 20 && tentfernung4 < pentfernung4 + 20 )) { //oke
      entfernung4 = (0.5 * entfernung4) + (0.5 * tentfernung4);
      wdt4 = 0;
      pentfernung4 = entfernung4;
      
    } else { //bullshit
      wdt4++;
    }
  }
  delay(25);

  //delay(100);

  Serial1.print("UA:");
  Serial1.print(entfernung1);
  Serial1.print(",UB:");
  Serial1.print(entfernung2);
  Serial1.print(",UC:");
  Serial1.print(entfernung3);
  Serial1.print(",UD:");
  Serial1.println(entfernung4);

  

}


void requestEvent() {		//Read = anforderung vom Main
  byte c = Wire.read();
  if (c == 4) {
    Wire.write(entfernung1);
    Wire.write(entfernung2);
    Wire.write(entfernung3);
    Wire.write(entfernung4);
  }
}



void configurePins() {
  pinMode(SD_A_TRIG, OUTPUT);
  pinMode(SD_A_ECHO, INPUT);
  pinMode(SD_B_TRIG, OUTPUT);
  pinMode(SD_B_ECHO, INPUT);
  pinMode(SD_C_TRIG, OUTPUT);
  pinMode(SD_C_ECHO, INPUT);
  pinMode(SD_D_TRIG, OUTPUT);
  pinMode(SD_D_ECHO, INPUT);

  pinMode(SW_A, INPUT_PULLUP);
  pinMode(SW_B, INPUT_PULLUP);
}
