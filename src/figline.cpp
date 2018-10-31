#include "figline.h"
#include <iostream>


FIGline::FIGline()
{
}

FIGline::FIGline(std::initializer_list<std::string> v)
{
    this->insert(this->begin(), v.begin(), v.end());
}

std::ostream& operator<<(std::ostream& os, FIGline& v)
{
    for (auto line : v) {
        os << line << '\n';
    }
    return os;
}
