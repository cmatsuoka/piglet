#ifndef SRC_LINES_H_
#define SRC_LINES_H_

#include <string>
#include <vector>


class Lines : public std::vector<std::string> {
 public:
    Lines();
    Lines(std::initializer_list<std::string>);
};

std::ostream& operator<<(std::ostream&, Lines&);


#endif  // SRC_LINES_H_
