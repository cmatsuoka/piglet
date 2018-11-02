#include "charsmush.h"
#include <string>
#include "figfont.h"

namespace {

    constexpr wchar_t cmp_return_other(const wchar_t a, const wchar_t b, const wchar_t c)
    {
        return (a == b) ? c : (a == c) ? b : 0;
    }
    
    wchar_t find_return_latter(std::wstring const& s1, std::wstring const& s2, const wchar_t a, const wchar_t b)
    {
        if (s1.find(a) != std::wstring::npos && s2.find(b) != std::wstring::npos) {
            return b;
        }
        if (s1.find(b) != std::wstring::npos && s2.find(a) != std::wstring::npos) {
            return a;
        }
        return 0;
    }
    
    constexpr wchar_t cmp_any_return(const wchar_t c1, const wchar_t c2, const wchar_t a, const wchar_t b, const wchar_t r)
    {
        return ((a == c1 && b == c2) || (a == c2 && b == c1)) ? r : 0;
    }

    constexpr wchar_t cmp_return(const wchar_t c1, const wchar_t c2, const wchar_t a, const wchar_t b, const wchar_t r)
    {
        return (a == c1 && b == c2) ? r : 0;
    }
    
    // Rule 1: EQUAL CHARACTER SMUSHING (code value 1)
    // Two sub-characters are smushed into a single sub-character if they are the same (except
    // hardblanks). 
    constexpr wchar_t rule_1(const wchar_t l, const wchar_t r, const uint32_t mode)
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
    wchar_t rule_2(const wchar_t l, const wchar_t r, const uint32_t mode)
    {
        if (mode & SmushUnderline) {
            wchar_t ret;
            if ((ret = find_return_latter(L"_", LR"#(|/\[]{}()<>)#", l, r)) != 0) {
                return ret;
            }
        }
        return 0;
    }
    
    // Rule 3: HIERARCHY SMUSHING (code value 4)
    // A hierarchy of six classes is used: "|", "/\", "[]", "{}", "()", and "<>". When two
    // smushing sub-characters are from different classes, the one from the latter class will
    // be used.
    wchar_t rule_3(const wchar_t l, const wchar_t r, const uint32_t mode)
    {
        if (mode & SmushHierarchy) {
            wchar_t ret;
            if ((ret = find_return_latter(L"|", LR"#(/\[]{}()<>)#", l, r)) != 0) {
                return ret;
            }
            if ((ret = find_return_latter(LR"(/\)", L"[]{}()<>", l, r)) != 0) {
                return ret;
            }
            if ((ret = find_return_latter(L"[]", L"{}()<>", l, r)) != 0) {
                return ret;
            }
            if ((ret = find_return_latter(L"{}", L"()<>", l, r)) != 0) {
                return ret;
            }
            if ((ret = find_return_latter(L"()", L"<>", l, r)) != 0) {
                return ret;
            }
        }
        return 0;
    }
    
    // Rule 4: OPPOSITE PAIR SMUSHING (code value 8)
    // Smushes opposing brackets ("[]" or "]["), braces ("{}" or "}{") and parentheses ("()"
    // or ")(") together, replacing any such pair with a vertical bar ("|").
    wchar_t rule_4(const wchar_t l, const wchar_t r, const uint32_t mode)
    {
        if (mode & SmushPair) {
            wchar_t ret;
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
    wchar_t rule_5(const wchar_t l, const wchar_t r, const uint32_t mode)
    {
        if (mode & SmushBigX) {
            wchar_t ret;
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


wchar_t CharSmusher::smush(const wchar_t l, const wchar_t r, const wchar_t hardblank, bool right2left, uint32_t mode)
{
    wchar_t ret;
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

