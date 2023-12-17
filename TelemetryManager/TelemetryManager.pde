import processing.serial.*;


Serial telem;
String subMode = "-";
String Mode = "-";
String dist_f = "-";
String dist_r = "-";
String dist_l = "-";
String dist_b = "-";
String lastRes = "-";
boolean prev;
String LastResponse = "-";
String lastCmd = "-";

PImage graphic = new PImage();

void setup(){
    size(600,310);
    telem = new Serial(this, Serial.list()[0], 115200);
    printArray(Serial.list());
    graphic = loadImage("IDLE.png");
}
void draw(){
    background(0);
    image(graphic,250,10);
    
    textSize(30);
    fill(255,160,0);
    text("Dashboard V1.4\n---------------------",30,60);

    textSize(20);
    fill(0,255,0);
    text("Mode: "+Mode+"\nsubMode:"+subMode+"\nlast Recieved:"+lastRes,30,120);
    fill(0,255,255);
    text("Left   : "+dist_l+" cm\nRight: "+dist_r+" cm\nFWD  : "+dist_f+" cm\nBack : "+dist_b+" cm\n",30,200);

    if(telem.available() > 0){
      
      String msg = telem.readString();
      println(msg);
      String vals[] = msg.split("#");
      
      for(int i = 0; i < vals.length; i++){
        String secs[] = vals[i].split(":");
        try{
        if(secs[0].equals("SM")){
            subMode = secs[1];
        }else if(secs[0].equals("M")){
            Mode = secs[1];
        }else if(secs[0].equals("DF")){
            dist_f = secs[1];
        }else if(secs[0].equals("DB")){
            dist_b = secs[1];
        }else if(secs[0].equals("DL")){
            dist_l = secs[1];
        }else if(secs[0].equals("DR")){
            dist_r = secs[1];
        }else if(secs[0].equals("LR")){
            lastRes = secs[1];
        }
        }catch(Exception e){
          println("an error occured");
        }
        
      }
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
    graphic = loadImage(cmd+".png");
}
