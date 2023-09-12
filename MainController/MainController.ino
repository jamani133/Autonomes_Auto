#include "defMap.cpp"
#include "vars.cpp"
#include "Adafruit_NeoPixel.h"


void setup(){
    configurePins();
    Adafruit_NeoPixel leds(LED_NUM, LED, NEO_GRB + NEO_KHZ800);
    leds.begin();
}

void loop(){
    
}