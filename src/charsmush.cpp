#include "charsmush.h"
#include <string>
#include "figfont.h"

namespace {

    constexpr char cmp_return_other(const char a, const char b, const char c)
    {
        return (a == b) ? c : (a == c) ? b : 0;
    }
    
    char find_return_latter(std::string const& s1, std::string const& s2, const char a, const char b)
    {
        if (s1.find(a) != std::string::npos && s2.find(b) != std::string::npos) {
            return b;
        }
        if (s1.find(b) != std::string::npos && s2.find(a) != std::string::npos) {
            return a;
        }
        return 0;
    }
    
    constexpr char cmp_any_return(const char c1, const char c2, const char a, const char b, const char r)
    {
        return ((a == c1 && b == c2) || (a == c2 && b == c1)) ? r : 0;
    }

    constexpr char cmp_return(const char c1, const char c2, const char a, const char b, const char r)
    {
        return (a == c1 && b == c2) ? r : 0;
    }
    
    // Rule 1: EQUAL CHARACTER SMUSHING (code value 1)
    // Two sub-characters are smushed into a single sub-character if they are the same (except
    // hardblanks). 
    constexpr char rule_1(const char l, const char r, const uint32_t mode)
    {
        if (mode & SmushEqual) {
            if (l == r) {
                return l;
            }
        }
        return 0;
    }
    
    // Rule 2: UNDERSCORE SMUSHING (code value 2)
    // An underscore ("_") will be replaced by any of: "|", "/", "\", "[", "]", "{", "}", "(",
    // ")", "<" or ">".
    char rule_2(const char l, const char r, const uint32_t mode)
    {
        if (mode & SmushUnderline) {
            char ret;
            if ((ret = find_return_latter("_", R"#(|/\[]{}()<>)#", l, r)) != 0) {
                return ret;
            }
        }
        return 0;
    }
    
    // Rule 3: HIERARCHY SMUSHING (code value 4)
    // A hierarchy of six classes is used: "|", "/\", "[]", "{}", "()", and "<>". When two
    // smushing sub-characters are from different classes, the one from the latter class will
    // be used.
    char rule_3(const char l, const char r, const uint32_t mode)
    {
        if (mode & SmushHierarchy) {
            char ret;
            if ((ret = find_return_latter("|", R"#(/\[]{}()<>)#", l, r)) != 0) {
                return ret;
            }
            if ((ret = find_return_latter(R"(/\)", "[]{}()<>", l, r)) != 0) {
                return ret;
            }
            if ((ret = find_return_latter("[]", "{}()<>", l, r)) != 0) {
                return ret;
            }
            if ((ret = find_return_latter("{}", "()<>", l, r)) != 0) {
                return ret;
            }
            if ((ret = find_return_latter("()", "<>", l, r)) != 0) {
                return ret;
            }
        }
        return 0;
    }
    
    // Rule 4: OPPOSITE PAIR SMUSHING (code value 8)
    // Smushes opposing brackets ("[]" or "]["), braces ("{}" or "}{") and parentheses ("()"
    // or ")(") together, replacing any such pair with a vertical bar ("|").
    char rule_4(const char l, const char r, const uint32_t mode)
    {
        if (mode & SmushPair) {
            char ret;
            if ((ret = cmp_any_return('[', ']', l, r, '|')) != 0) {
                return ret;
            }
            if ((ret = cmp_any_return('{', '}', l, r, '|')) != 0) {
                return ret;
            }
            if ((ret = cmp_any_return('(', ')', l, r, '|')) != 0) {
                return ret;
            }
        }
        return 0;
    }
    
    // Rule 5: BIG X SMUSHING (code value 16)
    // Smushes "/\" into "|", "\/" into "Y", and "><" into "X". Note that "<>" is not smushed
    // in any way by this rule. The name "BIG X" is historical; originally all three pairs
    // were smushed into "X".
    char rule_5(const char l, const char r, const uint32_t mode)
    {
        if (mode & SmushBigX) {
            char ret;
            if ((ret = cmp_return('/', '\\', l, r, '|')) != 0) {
                return ret;
            }
            if ((ret = cmp_return('\\', '/', l, r, 'Y')) != 0) {
                return ret;
            }
            if ((ret = cmp_return('>', '<', l, r, 'X')) != 0) {
                return ret;
            }
        }
        return 0;
    }

}  // namespace


char CharSmusher::smush(const char l, const char r, const char hardblank, bool right2left, uint32_t mode)
{
    char ret;
    if ((ret = cmp_return_other(' ', l, r)) != 0) {
        return ret;
    }

    // Kerning mode
    if (mode & SmushKern) {
        return 0;
    }

    // Universal smushing simply overrides the sub-character from the earlier
    // FIGcharacter with the sub-character from the later FIGcharacter. This
    // produces an "overlapping" effect with some FIGfonts, wherin the latter
    // FIGcharacter may appear to be "in front".
    if (mode == 0) {
        // Conserve spacing between words
        if (l == hardblank || r == hardblank) {
            return 0;
        }

        // Ensure that the dominant (foreground) fig-character for overlapping is
        // the latter in the user's text, not necessarily the rightmost character
        if (right2left) {
            return l;
        }

        return r;
    }

    // Rule 6: HARDBLANK SMUSHING (code value 32)
    // Smushes two hardblanks together, replacing them with a single hardblank.
    if (l == hardblank || r == hardblank) {
        return (mode & SmushHardblank) ? l : 0;
    }

    if ((ret = rule_1(l, r, mode)) != 0) {
        return ret;
    } else if ((ret = rule_2(l, r, mode)) != 0) {
        return ret;
    } else if ((ret = rule_3(l, r, mode)) != 0) {
        return ret;
    } else if ((ret = rule_4(l, r, mode)) != 0) {
        return ret;
    } else if ((ret = rule_5(l, r, mode)) != 0) {
        return ret;
    }

    return 0;
}

