#include <iostream>
#include "byte.h"

int main(int argc, char** argv)
{
    jm::Bytes b1{ 5 };
    b1[0] = 'a';
    b1[1] = 'b';
    b1[2] = 'c';
    b1[3] = 'd';
    b1[4] = 'e';
    jm::Bytes::View b2{ b1.makeView(1, 2) };
    jm::Bytes::Handle b3{ b1.makeHandle(1, 2) };

    jm::Bytes b4{ 0 };

    std::cout << b4.end() << std::endl;

    return 0;
}