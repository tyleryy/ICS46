#include "multiline.hpp"

multiline::multiline(){}

multiline::multiline(int max, int register_num, int total_time, std::vector<int> process_times, int curr_time, std::vector<std::pair<int, int>*> custTimes)
{
    cust_times = custTimes;
    current_time = curr_time;
    tot_time = total_time;
    for(int i = 0; i < register_num; i++)
    {
        Queue<std::shared_ptr<Customer>>* new_queue{new Queue<std::shared_ptr<Customer>>()};
        dest_vec.push_back(new_queue);
        std::shared_ptr<Register> new_register(new Register{new_queue, i+1, max, process_times.at(i), current_time}); 
        registers.push_back(new_register);
    }
}

multiline::~multiline()
{
    for(auto i : dest_vec)
    {
        delete i;
    }
    
}

void multiline::simulate()
{
    for(current_time = 0; current_time != tot_time; current_time++)
    {
        update_registers();
        if ((cust_times.size() != 0) && (current_time == cust_times.at(0)->second) )
        {
            int cust_num = cust_times.at(0)->first;
            multi_lines(cust_num);
            cust_times.erase(cust_times.begin());
        }
        check_all_if_dequeue();
    }
}

void multiline::get_stats()
{
    double avg_wait_time;
    for(std::shared_ptr<Register> r: registers)
    {

        tot_entered += r->total_entered;
        tot_exited += r->total_exited;
        tot_exited_reg += r->total_exited_reg;
        tot_lost += r->total_lost;
        tot_wait_time += r->total_wait_time;
        if(not r->isEmpty())
        {
            left_at_register++;
            left_in_line += r->reg_line_size();
        }
    }
    avg_wait_time = ((double) tot_wait_time)/(tot_exited);
    printf("STATS\n");
    printf("Entered Line    : %d\n", tot_entered);
    printf("Exited Line     : %d\n", tot_exited);
    printf("Exited Register : %d\n", tot_exited_reg);
    printf("Avg Wait Time   : %.2f\n", avg_wait_time);
    printf("Left In Line    : %d\n", left_in_line);
    printf("Left In Register: %d\n", left_at_register);
    printf("Lost            : %d\n", tot_lost);
}

void multiline::update_registers()
{
    for (std::shared_ptr<Register> r: registers)
    {
        r->set_time(current_time);
    }
}

void multiline::multi_lines(int cust_num)
{
    for (int i = 0; i < cust_num; i++)
    {
        std::shared_ptr<Customer> a_customer(new Customer{true, current_time, 0});
        std::shared_ptr<Register> open_register = get_open_register();
        
        open_register->add_to_line(a_customer);
    }
    for (std::shared_ptr<Register> r: registers)
    {
        if(r->reg_is_empty() && r->queue_size() >= 1) 
        {
            r->set_front_as_register();
        }
    }
}

std::shared_ptr<Register> multiline::get_open_register()
{
    std::vector<int> register_sizes; 
    for (std::shared_ptr<Register> r : registers)
    {
        register_sizes.push_back(r->reg_line_size());
    }
    int min_register_index = min_element(register_sizes.begin(), register_sizes.end()) - register_sizes.begin();
    return registers[min_register_index];
}

void multiline::check_all_if_dequeue()
{
    for (auto r: registers)
    {
        r->check_if_dequeue();
    }
}