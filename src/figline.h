#ifndef SRC_FIGLINE_H_
#define SRC_FIGLINE_H_

#include <string>
#include <vector>


class FIGline : public std::vector<std::wstring> {
 public:
    FIGline();
    FIGline(int);
    FIGline(std::initializer_list<std::wstring>);
};

std::wostream& operator<<(std::wostream&, FIGline);


#endif  // SRC_FIGLINE_H_
