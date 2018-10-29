#ifndef SRC_FIGCHAR_H_
#define SRC_FIGCHAR_H_

#include <string>
#include <vector>
#include "lines.h"
#include "file_io.h"


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
