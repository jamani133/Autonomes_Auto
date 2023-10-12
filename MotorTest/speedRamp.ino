int speedRamp(int cur, int maxSpeed, int rampup, int rampdown, int duration){
    if(cur >= rampup){
        if(cur>=duration-rampdown){
            return(int(map(cur,duration-rampdown,duration,maxSpeed,0))); //down
        }else{
            return(maxSpeed); //hold
        }
    }else{
        //up
        return(int(map(cur,0,rampup,0,maxSpeed)));
    }
}