#ifndef SRC_FIGCHAR_H_
#define SRC_FIGCHAR_H_

#include <string>
#include <vector>
#include "file_io.h"

using Lines = std::vector<std::string>;


class FIGchar {
    Lines lines;
 public:
    FIGchar();
    FIGchar(int);
    FIGchar(Lines);
    FIGchar(InputFile&, int);
    Lines const& get() { return lines; }
    bool operator==(FIGchar const&) const;
    bool operator!=(FIGchar const&) const;
};

std::ostream& operator<<(std::ostream&, FIGchar&);


#endif  // SRC_FIGCHAR_H_
