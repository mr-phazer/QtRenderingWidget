#pragma once
#include <string.h>


// Makes unordered map keys case insensitive
struct CaseInsensitiveUnorderedMapComparer {
    struct comp {
        bool operator() (const std::string& lhs, const std::string& rhs) const {
            // On non Windows OS, use the function "strcasecmp" in #include <strings.h>
            return _stricmp(lhs.c_str(), rhs.c_str()) == 0;
        }
    };
    struct hash {
        std::size_t operator() (std::string str) const {
            for (std::size_t index = 0; index < str.size(); ++index) {
                auto ch = static_cast<unsigned char>(str[index]);
                str[index] = static_cast<unsigned char>(std::tolower(ch));
            }
            return std::hash<std::string>{}(str);
        }
    };
};