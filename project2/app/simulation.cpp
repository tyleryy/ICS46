#include "simulation.hpp"

Simulation::Simulation()
{
    input_setup();
    printf("LOG\n");
    printf("%d start\n", curr_time);
    run();
    printf("%d end\n", tot_time);
    printf("\n");
    get_stats();
}

Simulation::~Simulation()
{
    if (line_type == "S")
    {
        delete SingleLine;
    }
    else
    {
        delete MultiLine;
    }
    for (auto c : cust_times_pairs)
    {
        delete c;
    }
}

void Simulation::get_stats()
{
    if (line_type == "S")
    {
        SingleLine->get_stats();
    }
    else
    {
        MultiLine->get_stats();
    }
}

void Simulation::input_setup()
{
    int register_num, max, process_time;
    std::string num_of_cust, arrival_time;
    std::vector<int> process_times;
    std::cin >> tot_time;
    std::cin >> register_num;
    std::cin >> max;
    std::cin >> line_type;
    tot_time*=60;
    for(int i = 0; i < register_num; i++)
    {
        std::cin >> process_time;
        process_times.push_back(process_time);
    }
    while (true)
    {
        std::cin >> num_of_cust;
        if (num_of_cust == "END")
            break;
        std::cin >> arrival_time;
        int first = std::stoi(num_of_cust);
        int second = std::stoi(arrival_time);
        std::pair<int, int>* pair{new std::pair<int, int>{first, second}};
        cust_times_pairs.push_back(pair);
    }

    if (line_type == "S")
    {
        SingleLine = new singleline(max, register_num, tot_time, process_times, curr_time, cust_times_pairs);
    }
    else
    {
        MultiLine = new multiline(max, register_num, tot_time, process_times, curr_time, cust_times_pairs);
    }

}

void Simulation::run()
{
    if (line_type == "S")
    {
        SingleLine->simulate();
    }
    else
    {
        MultiLine->simulate();
    }
}
