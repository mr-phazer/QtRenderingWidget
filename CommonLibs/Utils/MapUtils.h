#pragma once
#include <string>
#include <cwctype>
#include <unordered_map>

namespace utils {

    // Makes unordered map keys case insensitive
    struct WCaseInsensitiveUnorderedMapComparer {
        struct Compare {
            bool operator() (const std::wstring& lhs, const std::wstring& rhs) const {
                // On non Windows OS, use the function "strcasecmp" in #include <strings.h>
                return _wcsicmp(lhs.c_str(), rhs.c_str()) == 0;
            }
        };
        struct Hash {
            std::size_t operator() (std::wstring str) const {

                for (wchar_t& itCharacter : str)
                    itCharacter = static_cast<wchar_t>(std::towlower(static_cast<std::wint_t>(itCharacter)));

                //for (std::size_t index = 0; index < str.size(); ++index) {

                //    str[index] = static_cast<wchar_t>(std::tolower(static_cast<std::wint_t>(str[index])));
                //}
                return std::hash<std::wstring>{}(str);
            }
        };
    };

    // Makes unordered map keys case insensitive
    struct CaseInsensitiveUnorderedMapComparer {
        struct Compare {
            bool operator() (const std::string& lhs, const std::string& rhs) const {
                // On non Windows OS, use the function "strcasecmp" in #include <strings.h>
                return _stricmp(lhs.c_str(), rhs.c_str()) == 0;
            }
        };
        struct Hash {
            std::size_t operator() (std::string str) const {
                for (std::size_t index = 0; index < str.size(); ++index) {
                    auto ch = static_cast<unsigned char>(str[index]);
                    str[index] = static_cast<unsigned char>(std::tolower(ch));
                }
                return std::hash<std::string>{}(str);
            }
        };
    };

    /// <summary>
    ///  Map using  case insensitive strings as a keys
    /// For string
    /// </summary>
    /// <typeparam m_nodeName="VALUE"></typeparam>
    template <typename VALUE>
    using StringkeyMap
    = std::unordered_map<
        std::string,
        VALUE,
        CaseInsensitiveUnorderedMapComparer::Hash,
        CaseInsensitiveUnorderedMapComparer::Compare>;

    /// <summary>
    ///  Map using  case insensitive strings as a keys
    /// For wstring
    /// </summary>
    /// <typeparam m_nodeName="VALUE"></typeparam>
    template <typename VALUE>
    using WStringkeyMap = std::unordered_map<
        std::wstring,
        VALUE,
        WCaseInsensitiveUnorderedMapComparer::Hash,
        WCaseInsensitiveUnorderedMapComparer::Compare>;
}
