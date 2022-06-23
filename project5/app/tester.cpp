#include <iostream>
#include <vector>

int main()
{
    std::vector<int> cringe {1,3,4};
    for(const auto& i : cringe)
    {
        std::cout << i << std::endl;
    }
}