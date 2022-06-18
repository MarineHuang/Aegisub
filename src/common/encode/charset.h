

#pragma once

#include <string>

namespace SQ
{

bool IsCJK(char32_t c);

#ifdef _WIN32
std::string UTF8ToGBK(const std::string& strUTF8);
std::string GBKToUTF8(const std::string& strGBK);
#endif
    
} // end namespace SQ

