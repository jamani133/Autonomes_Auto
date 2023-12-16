import processing.serial.*;


Serial telem;

boolean prev;


void setup(){
    size(900,700);
    telem = new Serial(this, Serial.list()[0], 115200);
    printArray(Serial.list());
}
void draw(){
    background(0);
    
    textSize(30);
    fill(255,160,0);
    text("Dashboard V1.2\n---------------",30,30);

    textSize(20);
    fill(0,255,0);
    text("",30,30);

    if(telem.available() > 0){
      print(telem.readString());
    }
    
    if(keyPressed && !prev){
        if(key == 'w'){
            setDir("fwd");
        }
        if(key == 'a'){
            setDir("rleft");
        }
        if(key == 's'){
            setDir("back");
        }
        if(key == 'd'){
            setDir("rright");
        }
        if(key == 'e'){
            setDir("right");
        }
        if(key == 'p'){
            setDir("AUTO");
        }
        if(key == 'r'){
            setDir("RAMPAGE1");
        }
        if(key == 'q'){
            setDir("left");
        }
        if(key == 'x'){
            setDir("IDLE");
        }
    }
    prev = keyPressed;
}

void setDir(String cmd){
    telem.write(cmd+'\n');
    lastCmd = cmd;
}