#include <iostream>

#include "advanced_cxx/cmplx_num.h"

int main()
{
    mabz::Cmplx c1(1, 2);
    mabz::Cmplx c2(-2, -3);
    std::cout << c1 + c2 << std::endl;
    std::cout << c2 / c1 << std::endl;
    return 0;
}