
#pragma once

#include <string>
#include <vector>

namespace SQ
{
    
    template <class T>
    std::string Join(T& val, std::string delim);
    
    std::vector<std::string> SplitString(const std::string& str, 
        const std::string& seps, 
        bool remove_empty_entries = false);
    
    std::vector<std::wstring> SplitStringW(const std::wstring& str, 
        const std::wstring& seps, 
        bool remove_empty_entries = false);

    void StringReplace(std::string &src, const std::string &s1, const std::string &s2);
    
    std::string LeftTrim(const char* source);
    std::string RightTrim(const char* source);
    std::string Trim(const char* source);
    std::string LeftTrimQuotes(const char* source);
    std::string RightTrimQuotes(const char* source);
    std::string TrimQuotes(const char* source);
    std::string ToLower(const char* source);
    std::string ToUpper(const char* source);

}
