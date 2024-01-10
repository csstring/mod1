#include "Common.h"
#include <sstream>

void ft_assert(const std::string& exec)
{
    std::cout << exec << std::endl;
    assert(false);
}