#include "strsmush.h"
#include <iostream>
#include <cstdint>
#include <algorithm>
#include "charsmush.h"


// Compute the number of characters a string can be smushed into another string.
int StringSmusher::amount(std::wstring const& s1, std::wstring const& s2, wchar_t hardblank, uint32_t mode)
{
    int amt = 0;

    char l = ' ';
    int lpos = std::find_if(s1.rbegin(), s1.rend(), [](int ch) { return !std::isspace(ch); }) - s1.rbegin();
    if (lpos < s1.length()) {
        l = s1[s1.length() - 1 - lpos];
    }
    amt += lpos;
    
    char r = ' ';
    int rpos = std::find_if(s2.begin(), s2.end(), [](int ch) { return !std::isspace(ch); }) - s2.begin();
    if (rpos < s2.length()) {
        r = s2[rpos];
    }
    amt += rpos; 

    if (l == ' ' || r == ' ') {
        return amt;
    }

    if (CharSmusher::smush(l, r, hardblank, false, mode) != 0) {
        amt++;
    }

    return amt;
}


std::wstring StringSmusher::smush(std::wstring const& s1, std::wstring const& ss2, int amt, wchar_t hardblank, uint32_t mode)
{
    if (ss2.empty()) {
        return s1;
    }

    int l1 = s1.length();

    std::wstring s2(ss2);

    if (amt > l1) {
        s2 = s2.substr(amt - l1, s2.length());
        amt = l1;
    }

    int l2 = s2.length();
    int m1 = l1 - amt;

    // part 1: only characters from s1
    // FIXME: we want characters not bytes
    auto res = s1.substr(0, m1);

    // part 2: s1 and s2 overlap
    for (int i = 0; i < l2; i++) {
        auto l = (m1 + i < s1.length()) ? s1[m1 + i] : ' ';
        auto r = s2[i];
        if (l != ' ' && r != ' ') {
            auto c = CharSmusher::smush(l, r, hardblank, false, mode);
            res += (c != 0) ? c : r;
        } else {
            res += (l == ' ') ? r : l;
        }
    }

    // part 3: remainder of s1 after the end of s2
    // FIXME: we want characters not bytes
    int m2 = m1 + l2;
    for (int i = m2; i < l1; i++) {
        res += s1[i];
    }

    return res;
}
