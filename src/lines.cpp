#include "lines.h"
#include <iostream>


Lines::Lines()
{
}

Lines::Lines(std::initializer_list<std::string> v)
{
    this->insert(this->begin(), v.begin(), v.end());
}

std::ostream& operator<<(std::ostream& os, Lines& v)
{
    for (auto line : v) {
        os << line << '\n';
    }
    return os;
}
