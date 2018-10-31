#ifndef SRC_FIGCHAR_H_
#define SRC_FIGCHAR_H_

#include <string>
#include <vector>
#include "figline.h"
#include "file_io.h"


class FIGchar {
    FIGline lines;
 public:
    FIGchar();
    FIGchar(int);
    FIGchar(FIGline);
    FIGchar(InputFile&, int);
    FIGline const& get() { return lines; }
    bool operator==(FIGchar const&) const;
    bool operator!=(FIGchar const&) const;
};

std::ostream& operator<<(std::ostream&, FIGchar&);


#endif  // SRC_FIGCHAR_H_
