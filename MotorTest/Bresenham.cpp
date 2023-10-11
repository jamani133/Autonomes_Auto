class Bresenham{
    public:
        boolean queueStep = false;
        float timeFrame = 1000; //1s
        float steps = 3200;
    private:
        float steps_per_us = steps/timeFrame;
        int cur_us = 0;
        int cur_steps = 0;

    void config(float steps_num = 0, float time = 1000000){
        steps_per_us = steps/timeFrame;
        cur_us = 0;
        cur_steps = 0;
        queueStep = false;
    }
        
    void advance(){
        cur_us++;
        float cur_y = float(cur_us)*steps_per_us;
        if(cur_y - float(cur_steps) > 0.5){
            cur_steps++;
            queueStep = true;
        }
    }
}