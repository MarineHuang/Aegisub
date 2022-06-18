#include <algorithm>
#include <cstring>
#include "utils.h"

namespace SQ
{

/**  
 * @brief 将容器中的多个字符串合并成一个字符串 
 * @param  T& val 容器
 * @param  std::string delim 分隔符
 * @return 合并后的字符窗
 * @code
 * std::vector<std::string> str_vec;
 * str_vec.push_back("ab");
 * str_vec.push_back("cd");         
 * auto str = SQ::Join(str_vec, "-")
 * @endcode 
 * @see 
 */
template <class T>
std::string Join(T& val, std::string delim)
{
    std::string str;
    typename T::iterator it;
    const typename T::iterator itlast = val.end()-1;
    for (it = val.begin(); it != val.end(); it++)
    {
        str += *it;
        if (it != itlast)
        {
            str += delim;
        }
    }
    return str;
}

std::vector<std::string> SplitString(const std::string& str, 
    const std::string& seps, 
    bool remove_empty_entries) 
{
  std::vector<std::string> result;
  std::string::size_type pre_pos = 0;

  //TODO: bug fix
  while (true) 
  {
    auto next_pos = str.find_first_of(seps, pre_pos);

    if (next_pos == std::string::npos) 
    {
      auto sub_str = str.substr(pre_pos, next_pos);
      // sub_str is empty means the last sep reach the end of string
      if (!sub_str.empty()) {
        result.push_back(sub_str);
      }

      break;
    }

    if (pre_pos != next_pos || !remove_empty_entries) 
    {
      auto sub_str = str.substr(pre_pos, next_pos - pre_pos);
      result.push_back(sub_str);
    }

    pre_pos = next_pos + 1;
  }

  return result;
}

std::vector<std::wstring> SplitStringW(const std::wstring& str, 
    const std::wstring& seps, 
    bool remove_empty_entries) 
{
  std::vector<std::wstring> result;
  std::wstring::size_type pre_pos = 0;

  //TODO: bug fix
  while (true) 
  {
    auto next_pos = str.find_first_of(seps, pre_pos);

    if (next_pos == std::wstring::npos) 
    {
      auto sub_str = str.substr(pre_pos, next_pos);
      // sub_str is empty means the last sep reach the end of string
      if (!sub_str.empty()) {
        result.push_back(sub_str);
      }

      break;
    }

    if (pre_pos != next_pos || !remove_empty_entries) 
    {
      auto sub_str = str.substr(pre_pos, next_pos - pre_pos);
      result.push_back(sub_str);
    }

    pre_pos = next_pos + 1;
  }

  return result;
}

void StringReplace(std::string & src, const std::string & s1, const std::string & s2)
{
    std::string::size_type pos =0;
    while ((pos = src.find(s1, pos)) != std::string::npos) 
    {
        src.replace(pos, s1.length(), s2);
        pos += s2.length(); 
    }
}

std::string LeftTrim(const char* source)
{
    std::string copy(source);
    copy.erase(copy.begin(), std::find_if(copy.begin(), copy.end(), [](unsigned char ch) { return !::isspace(ch); }));
    return copy;
}

std::string RightTrim(const char* source)
{
    std::string copy(source);
    copy.erase(std::find_if(copy.rbegin(), copy.rend(), [](unsigned char ch) { return !::isspace(ch); }).base(), copy.end());
    return copy;
}

std::string Trim(const char* source)
{
    return LeftTrim(RightTrim(source).c_str());
}

std::string LeftTrimQuotes(const char* source)
{
    std::string copy(source);
    copy.erase(copy.begin(), std::find_if(copy.begin(), copy.end(), [](int ch) { return !(ch == '"'); }));
    return copy;
}

std::string RightTrimQuotes(const char* source)
{
    std::string copy(source);
    copy.erase(std::find_if(copy.rbegin(), copy.rend(), [](int ch) { return !(ch == '"'); }).base(), copy.end());
    return copy;
}

std::string TrimQuotes(const char* source)
{
    return LeftTrimQuotes(RightTrimQuotes(source).c_str());
}

std::string ToLower(const char* source)
{
    std::string copy;
    if (source) {
        size_t srcLength = strlen(source);
        copy.resize(srcLength);
        std::transform(source, source + srcLength, copy.begin(), [](unsigned char c) { return (char)::tolower(c); });
    }
    return copy;
}

std::string ToUpper(const char* source)
{
    std::string copy;
    if (source) {
        size_t srcLength = strlen(source);
        copy.resize(srcLength);
        std::transform(source, source + srcLength, copy.begin(), [](unsigned char c) { return (char)::toupper(c); });
    }
    return copy;
}


} // end namespace SQ
