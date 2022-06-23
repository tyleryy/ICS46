#include "register.hpp"

Register::Register(Queue<std::shared_ptr<Customer>>* q, int id, int max, int process_time, int curr_time)
{
    reg_q = q;
    reg_id = id;
    max_line_length = max;
    checkout_time = process_time;
    current_time = curr_time;
}

int Register::reg_line_size()
{
    auto iter = reg_q->constIterator();
    int count = 0;
    while(iter.isPastEnd() == false)
    {
        if ((*iter.value()).isInLine == true)
            count++;
        iter.moveToNext();
    }
    return count;
}

int Register::queue_size()
{
    return reg_q->size();
}

void Register::enqueue(std::shared_ptr<Customer> c, Queue<std::shared_ptr<Customer>>& q)
{
    c->exitTime = current_time;
    c->isInLine = false;
    reg_empty = false;
    int wait_time = c->exitTime - c->enterTime;
    total_wait_time += wait_time;
    reg_q->enqueue(c);
    printf("%d exited line %d length %d wait time %d\n", current_time, 1, q.size()-1, wait_time);
    printf("%d entered register %d\n",current_time, reg_id);
}

void Register::add_to_line(std::shared_ptr<Customer> c)
{
    if (reg_q->size() < max_line_length+1)
    {
        reg_q->enqueue(c);
        c->enterTime = current_time;
        printf("%d entered line %d length %d\n", current_time, reg_id, reg_line_size());
        total_entered++;
    }
    else
    {
        printf("%d lost\n", current_time);
        total_lost++;
    }
}

void Register::dequeue_reg()
{
    if ((not isEmpty()) && (checkout_time == (current_time - (reg_q->front()->exitTime))))
    {
        reg_q->dequeue();
        total_exited_reg++;
        printf("%d exited register %d\n", current_time, reg_id);
        reg_empty = true;
    }
}

void Register::check_if_dequeue() 
{

    if (not isEmpty())
    {
        if (checkout_time == (current_time - (reg_q->front()->exitTime)))
        {
            reg_q->dequeue();
            total_exited_reg++;
            printf("%d exited register %d\n", current_time, reg_id);
            reg_empty = true;
            if (not isEmpty())
            {
                set_front_as_register();
            }
        }
    }
}

bool Register::isEmpty(){
    return reg_q->isEmpty();
}

std::shared_ptr<Customer> Register::get_cust_at_reg()
{
    return reg_q->front();
}

bool Register::Register::reg_is_empty()
{
    return reg_empty;
}

void Register::set_time(int time)
{
    current_time = time;
}

void Register::set_front_as_register()
{
    std::shared_ptr<Customer> reg_cust = reg_q->front();
    reg_cust->isInLine = false;
    reg_cust->exitTime = current_time;
    int wait_time = reg_cust->exitTime - reg_cust->enterTime;
    total_wait_time += wait_time;
    printf("%d exited line %d length %d wait time %d\n", current_time, reg_id, reg_q->size()-1, wait_time);
    printf("%d entered register %d\n",current_time, reg_id);
    total_exited++;
    reg_empty = false;
}

