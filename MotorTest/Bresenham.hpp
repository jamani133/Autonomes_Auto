class Bresenham{
    public:
    boolean queueStep = false;
    float steps = 3200.0;

    float us_per_step = steps/1.0;
    int cur_us = 0;
    int cur_steps = 0;

    void config(int steps_num = 0.0, int time = 1000000.0){
        us_per_step = float(time)/float(steps_num);
        cur_us = 0;
        cur_steps = 0;
        queueStep = false;
    }

    void advance(int adv){
        cur_us += adv;
        float cur_y = cur_us/us_per_step;
        if(cur_y - float(cur_steps) > 0.5){
            cur_steps++;
            queueStep = true;
        }
    }
};