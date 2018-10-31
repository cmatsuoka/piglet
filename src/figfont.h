#ifndef SRC_FIGFONT_H_
#define SRC_FIGFONT_H_

#include <unordered_map>
#include "figchar.h"

constexpr uint32_t SmushEqual     = 1 << 0;
constexpr uint32_t SmushUnderline = 1 << 1;
constexpr uint32_t SmushHierarchy = 1 << 2;
constexpr uint32_t SmushPair      = 1 << 3;
constexpr uint32_t SmushBigX      = 1 << 4;
constexpr uint32_t SmushHardblank = 1 << 5;
constexpr uint32_t SmushKern      = 1 << 6;
constexpr uint32_t SmushEnable    = 1 << 7;


class FIGfont {
    char version;        // font standard version (currently 'a')
    int baseline;        // number of lines from the baseline of a FIGcharacter
    int max_length;      // maximum length of any line describing a FIGcharacter
    int comment_lines;   // number of comment lines at the start of the file
    bool right_to_left;  // FIGcharacter sequence rendering direction
    int count;           // number of code-tagged FIGcharacters in this FIGfont
    std::unordered_map<wchar_t, FIGchar> chars;
 public:
    char hardblank;      // sub-character used to represent hardblanks
    int height;
    int old_layout;
    int layout;

    FIGfont();                          // construct empty FIGfont
    FIGfont(std::string const&);        // construct from path
    FIGchar get(wchar_t ch);            // retrieve corresponding FIGchar
    FIGfont& load(std::string const&);  // load font file 
    FIGfont& parse_header(std::wstring const&);  // validate and parse font file header
};


#endif  // SRC_FIGFONT_H
