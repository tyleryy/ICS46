#pragma once
#include "register.hpp"
#include "Queue.hpp"

class singleline
{

public:
    singleline();
    singleline(int max, int register_num, int total_time, std::vector<int> process_times, int curr_time, std::vector<std::pair<int, int>*> custTimes);    

    ~singleline();

    void simulate();

    void get_stats();

    std::vector<Queue<std::shared_ptr<Customer>>*> dest_vec; 

private:

    bool empty_reg_exists();
    void move_to_regs();

    void single_line(int cust_num);

    void update_registers();

    void dequeue_registers();

    std::vector<std::shared_ptr<Register>> registers;
    std::vector<std::pair<int, int>*> cust_times;
    int current_time;
    int tot_time;


    struct master_line
    {
        Queue<std::shared_ptr<Customer>>* master_queue;
        int max_length;

    } line;

    int tot_entered = 0;
    int tot_lost = 0;
    int tot_exited = 0;
    int tot_exited_reg = 0;
    int tot_wait_time = 0;
    int left_at_register = 0;
    int left_in_line = 0;


};