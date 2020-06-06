#include <iostream>
#include <vector>
#include <string>


int main()
{
    std::vector<std::string> msg {"This","is","autograd", "in", "C++"};
    
    for (const std::string& word : msg)
    {
        std::cout << word << " ";
    }
    std::cout << std::endl;
}