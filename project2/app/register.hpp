#pragma once
#include "Queue.hpp"
#include <vector>
#include <algorithm>
#include <string>
#include <memory>
#include <cstdio>
#include <iostream>

struct Customer{
    bool isInLine;
    int enterTime; 
    int exitTime;
};  

class Register
{
    public:
    Register(Queue<std::shared_ptr<Customer>>* q, int id, int max, int process_time, int curr_time);

    int reg_line_size();

    int queue_size();

    void enqueue(std::shared_ptr<Customer> c, Queue<std::shared_ptr<Customer>>& q);

    void add_to_line(std::shared_ptr<Customer> c);

    void dequeue_reg();

    void check_if_dequeue();

    bool isEmpty();

    bool reg_is_empty();

    std::shared_ptr<Customer> get_cust_at_reg();
    
    void set_time(int time);

    void set_front_as_register();

    private:
    int max_line_length;

    bool reg_empty = true;

    public:
    Queue<std::shared_ptr<Customer>>* reg_q;
    int checkout_time;

    int current_time = 0;
    int reg_id;
    int total_exited = 0;
    int total_exited_reg = 0;
    int total_lost = 0;
    int total_wait_time = 0;
    int total_entered = 0;

};

