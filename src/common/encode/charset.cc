
#include <cstring>
#include "charset.h"

#ifdef _WIN32
#include <windows.h>
#endif // _WIN32

namespace SQ
{

bool IsCJK(char32_t c) {
    return (c >= 0x4E00 && c <= 0x9FFF)
        || (c >= 0x3400 && c <= 0x4DBF)
        || (c >= 0x20000 && c <= 0x2A6DF)
        || (c >= 0x2A700 && c <= 0x2B73F)
        || (c >= 0x2B740 && c <= 0x2B81F)
        || (c >= 0x2B820 && c <= 0x2CEAF)
        || (c >= 0xF900 && c <= 0xFAFF)
        || (c >= 0x2F800 && c <= 0x2FA1F);
}

#ifdef _WIN32
std::string UTF8ToGBK(const std::string& strUTF8) 
{
    int len = MultiByteToWideChar(CP_UTF8, 0, strUTF8.c_str(), -1, NULL, 0);
    unsigned short * wszGBK = new unsigned short[len + 1];
    memset(wszGBK, 0, len * 2 + 2);

    MultiByteToWideChar(CP_UTF8, 0, (char*)strUTF8.c_str(), -1, (wchar_t*)wszGBK, len);

    len = WideCharToMultiByte(CP_ACP, 0, (wchar_t*)wszGBK, -1, NULL, 0, NULL, NULL);

    char *szGBK = new char[len + 1];
    memset(szGBK, 0, len + 1);
    WideCharToMultiByte(CP_ACP, 0, (wchar_t*)wszGBK, -1, szGBK, len, NULL, NULL);

    std::string strTemp(szGBK);
    delete[] szGBK;
    delete[] wszGBK;

    return strTemp;
}

std::string GBKToUTF8(const std::string &strGBK)
{
    std::string strOutUTF8 = "";
    wchar_t * str1;

    int n = MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, NULL, 0);

    str1 = new wchar_t[n];

    MultiByteToWideChar(CP_ACP, 0, strGBK.c_str(), -1, str1, n);

    n = WideCharToMultiByte(CP_UTF8, 0, str1, -1, NULL, 0, NULL, NULL);

    char * str2 = new char[n];
    WideCharToMultiByte(CP_UTF8, 0, str1, -1, str2, n, NULL, NULL);
    strOutUTF8 = str2;

    delete[] str1;
    str1 = NULL;
    delete[] str2;
    str2 = NULL;

    return strOutUTF8;
}
#endif

} // end namespace SQ