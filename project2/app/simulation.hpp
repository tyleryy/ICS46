#pragma once
#include "singleline.hpp"
#include "multiline.hpp"


class Simulation
{
    public:
    Simulation();
    ~Simulation();

    private:
    void get_stats();
    void input_setup();
    void run();
    
    private:
    int curr_time = 0;
    std::vector<std::pair<int, int>*> cust_times_pairs;
    int tot_time = 0;
    std::string line_type;
    singleline* SingleLine;
    multiline* MultiLine;
};



