#ifndef SRC_FIGLINE_H_
#define SRC_FIGLINE_H_

#include <string>
#include <vector>


class FIGline : public std::vector<std::string> {
 public:
    FIGline();
    FIGline(std::initializer_list<std::string>);
};

std::ostream& operator<<(std::ostream&, FIGline&);


#endif  // SRC_FIGLINE_H_
