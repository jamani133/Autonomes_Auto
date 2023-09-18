#include "defMap.cpp"

void setup(){
    configurePins();
}
void loop(){
    
}

boolean procStep(int steps,int step){
    int stackA = int(steps+1 / 2);
    int stackB = int(steps / 2);
    if(step >= 29){
        return step%30 < stackA;
    }else{
        return step%30 < stackB;
    }
}