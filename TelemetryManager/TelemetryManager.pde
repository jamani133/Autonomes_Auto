import processing.serial.*;


Serial telem;

boolean prev;


void setup(){
    size(240,240);
    telem = new Serial(this, Serial.list()[0], 115200);
    printArray(Serial.list());
}
void draw(){
    background(0);
    if(telem.available() > 0){
      print(telem.readString());
    }
    
    if(keyPressed && !prev){
        if(key == 'w'){
            telem.write("fwd\n");
        }
        if(key == 'a'){
            telem.write("rleft\n");
        }
        if(key == 's'){
            telem.write("back\n");
        }
        if(key == 'd'){
            telem.write("rright\n");
        }
        if(key == 'e'){
            telem.write("right\n");
        }
        if(key == 'q'){
            telem.write("left\n");
        }
        if(key == 'x'){
            telem.write("IDLE\n");
        }
    }
    prev = keyPressed;
}
