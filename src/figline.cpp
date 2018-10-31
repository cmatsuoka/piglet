#include "figline.h"
#include <iostream>


FIGline::FIGline()
{
}

FIGline::FIGline(int height) : std::vector<std::wstring>(height)
{
}

FIGline::FIGline(std::initializer_list<std::wstring> v)
{
    this->insert(this->begin(), v.begin(), v.end());
}

std::wostream& operator<<(std::wostream& os, FIGline& v)
{
    for (auto line : v) {
        os << line << '\n';
    }
    return os;
}
