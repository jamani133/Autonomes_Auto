import porocessing.serial.*;

Serial telem = new Serial();

Serial[] varNames;
Serial[] varVals;
Serial[] varWDT;


void setup(){
    size(1024,720);
    connect();
}
void draw(){
    background(0);
    if(Serial.available()){
        String inputString = readStringUntil("\n");
        readVars(inputString);
    }
}


void connect(){
    boolean connected = false;
    while(!connected){

    }
}

void readVars(String input){
    if(input == null || input == "" || input == "\n"){
        return;
    }
    String[] varBlocks = input.split("|");
    if (varBlocks.length == 0){
        return;
    }
    
    for(int i = 0; i < varBlock.lenght ; i++){

    }
}




boolean Button(String text,int posX,int posY,int wid,int hei){
    rect(posX,posY,wid,hei);
    textAlign(CENTER);
    text(text,posX+(wid/2),posY+(hei/2));
    if(!mousePressed){
        return false;
    }
    if(posX > mouseX){
        return false;
    }
    if(posX+wid < mouseX){
        return false;
    }
    if(posY > mouseY){
        return false;
    }
    if(posY+hei < mouseY){
        return false;
    }
    return true;
}