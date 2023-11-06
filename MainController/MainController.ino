#include <Wire.h>
byte data[4] = {0,0,0,30};
void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial1.begin(115200);
}

void loop() {
  delay(1000);

  Wire.beginTransmission(0x01);
  data[0] = random(255);
  data[1] = random(255);
  data[2] = random(255);
  Wire.write(data,4);
  Wire.endTransmission();
  Serial.println("sending...");
  Serial1.println("Fuck You   0:"+String(data[0])+" 1:"+String(data[1])+" 2:"+String(data[2])+" 3:"+String(data[3]));
}
void loop() {
	Wire.requestFrom(0x02, 1)
	
	while(Wire.available()){
		char c=Wire.read();
