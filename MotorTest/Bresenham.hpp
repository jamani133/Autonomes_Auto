class Bresenham{
    public:
    boolean queueStep = false;
    float steps = 3200.0;

    float Ksteps_per_us = steps/1.0;
    int cur_us = 0;
    int cur_steps = 0;

    void config(int steps_num = 0.0, int time = 1000000.0){
        Ksteps_per_us = float(1000*steps_num)/float(time);
        cur_us = 0;
        cur_steps = 0;
        queueStep = false;
    }

    void advance(int adv){
        cur_us += adv;
        float cur_y = (float(cur_us)*Ksteps_per_us)/1000;
        if(cur_y - float(cur_steps) > 0.5){
            cur_steps++;
            queueStep = true;
        }
    }
};