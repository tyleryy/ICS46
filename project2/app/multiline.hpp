#pragma once
#include "register.hpp"
#include "Queue.hpp"

class multiline
{

public:
    multiline();
    multiline(int max, int register_num, int total_time, std::vector<int> process_times, int curr_time, std::vector<std::pair<int, int>*> custTimes);    

    ~multiline();

    void simulate();

    void get_stats();


    std::vector<Queue<std::shared_ptr<Customer>>*> dest_vec; 
    
private:

    void update_registers();

    void multi_lines(int cust_num);

    std::shared_ptr<Register> get_open_register();

    void check_all_if_dequeue();

    int tot_entered = 0;
    int tot_lost = 0;
    int tot_exited = 0;
    int tot_exited_reg = 0;
    int tot_wait_time = 0;
    int left_at_register = 0;
    int left_in_line = 0;

    std::vector<std::shared_ptr<Register>> registers;
    std::vector<std::pair<int, int>*> cust_times;
    int current_time;
    int tot_time;

};