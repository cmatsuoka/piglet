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
    FIGchar(Lines&);
    FIGchar(InputFile&, int);
};


#endif  // SRC_FIGCHAR_H_
