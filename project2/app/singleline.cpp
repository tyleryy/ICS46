#include "singleline.hpp"

singleline::singleline(){}

singleline::singleline(int max, int register_num, int total_time, std::vector<int> process_times, int curr_time, std::vector<std::pair<int, int>*> custTimes)  
{
    cust_times = custTimes;
    current_time = curr_time;
    tot_time = total_time;
    Queue<std::shared_ptr<Customer>>* new_queue{new Queue<std::shared_ptr<Customer>>()}; 
    dest_vec.push_back(new_queue);
    line.master_queue = new_queue;
    line.max_length = max;
    for(int i = 0; i < register_num; i++)
    {
        Queue<std::shared_ptr<Customer>>* a_new_queue{new Queue<std::shared_ptr<Customer>>()};
        dest_vec.push_back(a_new_queue);
        std::shared_ptr<Register> new_register(new Register{a_new_queue, i+1, 1, process_times.at(i), current_time}); 
        registers.push_back(new_register);
    }
}

singleline::~singleline()
{
    for(Queue<std::shared_ptr<Customer>>* i : dest_vec)
    {
        delete i;
    }
}

void singleline::simulate()
{
    for(current_time = 0; current_time != tot_time; (current_time)++)
    {
        update_registers();
        dequeue_registers();
        if ((cust_times.size() != 0) && (current_time == cust_times.at(0)->second))
        {
            int cust_num = cust_times.at(0)->first;
            single_line(cust_num);
            cust_times.erase(cust_times.begin());
        }
        move_to_regs();
    }
}

void singleline::get_stats()
{
    double avg_wait_time;
    for(std::shared_ptr<Register> r: registers)
    {
        tot_exited_reg += r->total_exited_reg;
        tot_lost += r->total_lost;
        tot_wait_time += r->total_wait_time;
        if(not r->isEmpty())
        {
            left_at_register++;
        }
    }
    left_in_line = line.master_queue->size();
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


bool singleline::empty_reg_exists()
{
    for (auto r : registers)
    {
        if(r->isEmpty())
            return true;
    }
    return false;
}

void singleline::move_to_regs()
{
    while ((empty_reg_exists()) && (line.master_queue->size() != 0))
    {
        std::shared_ptr<Register> reg;
        for (auto r : registers)
        {
            if(r->isEmpty())
            {
                reg = r;
                break;
            }
        }
        reg->enqueue(line.master_queue->front(), *line.master_queue);
        line.master_queue->dequeue();
        tot_exited++;

    }
}

void singleline::single_line(int cust_num)
{
    for (int i = 0; i < cust_num; i++)
    {
        std::shared_ptr<Customer> a_customer(new Customer{true, current_time, 0});
        if (line.master_queue->size() < line.max_length)
        {
            a_customer->enterTime = current_time;
            line.master_queue->enqueue(a_customer);
            printf("%d entered line %d length %d\n", current_time, 1, line.master_queue->size());
            tot_entered++;
        }
        else
        {
            printf("%d lost\n", current_time);
            tot_lost++;
        }
    }
}

void singleline::update_registers()
{
    for (std::shared_ptr<Register> r: registers)
    {
        r->set_time(current_time);
    }
}

void singleline::dequeue_registers()
{
    for (std::shared_ptr<Register> r: registers)
    {
        if (not r->isEmpty())
        {
            r->dequeue_reg();
        }
    }
}

