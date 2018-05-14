#include <iostream>
#include "override_new.h"

int main()
{
    int *x = new int[5]{};
    delete[] x;
    std::cout << Memuse() << std::endl;

    int *p = new int{3};
    delete p;
    std::cout << Memuse() << std::endl;
    return 0;
}
