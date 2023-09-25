void setup() {
  Serial.begin(115200);
}

void loop() {
  int high = 0;
  for(int j = 0; j < 100; j++){
    int runner = 0;
    for(int i = 0; i < 100; i++){
      runner = runner + analogRead(A7);
    }
    if(runner > 100){
      high++;
    }
    delay(100);
    Serial.print(".");
  }
  Serial.println();
  if(high > 90){
    Serial.print("100%   ");
  }else if(high > 30){
    Serial.print(">85%   ");
  }else{
    Serial.print("<85%   ");
  }
  Serial.println(high);
}
