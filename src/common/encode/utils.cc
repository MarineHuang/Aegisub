#include <algorithm>
#include <sstream>
#include "utils.h"

namespace SQ
{

std::string UrlEncode(const std::string & src)
{
    std::stringstream dest;
    static const char *hex = "0123456789ABCDEF";
    unsigned char c;

    for (size_t i = 0; i < src.size(); i++) {
        c = src[i];
        if (isalnum(c) || (c == '-') || (c == '_') || (c == '.') || (c == '~')) {
            dest << c;
        } else if (c == ' ') {
            dest << "%20";
        } else {
            dest << '%' << hex[c >> 4] << hex[c & 15];
        }
    }

    return dest.str();
}

std::string UrlDecode(const std::string & src)
{
    std::stringstream unescaped;
    unescaped.fill('0');
    unescaped << std::hex;

    size_t safeLength = src.size();
    const char *safe = src.c_str();
    for (auto i = safe, n = safe + safeLength; i != n; ++i)
    {
        char c = *i;
        if(c == '%')
        {
            char hex[3];
            hex[0] = *(i + 1);
            hex[1] = *(i + 2);
            hex[2] = 0;
            i += 2;
            auto hexAsInteger = strtol(hex, nullptr, 16);
            unescaped << (char)hexAsInteger;
        }
        else
        {
            unescaped << *i;
        }
    }

    return unescaped.str();
}

std::string Base64Encode(const std::string &src)
{
    return Base64Encode(src.c_str(), static_cast<int>(src.size()));
}

std::string Base64Encode(const ByteBuffer& buffer)
{
    return Base64Encode(reinterpret_cast<const char*>(buffer.data()), static_cast<int>(buffer.size()));
}

std::string Base64Encode(const char *src, int len)
{
    if (!src || len == 0) {
        return "";
    }

    static const char *ENC = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    auto in = reinterpret_cast<const unsigned char *>(src);
    auto inLen = len;
    std::stringstream ss;
    while (inLen) {
        // first 6 bits of char 1
        ss << ENC[*in >> 2];
        if (!--inLen) {
            // last 2 bits of char 1, 4 bits of 0
            ss << ENC[(*in & 0x3) << 4];
            ss << '=';
            ss << '=';
            break;
        }
        // last 2 bits of char 1, first 4 bits of char 2
        ss << ENC[((*in & 0x3) << 4) | (*(in + 1) >> 4)];
        in++;
        if (!--inLen) {
            // last 4 bits of char 2, 2 bits of 0
            ss << ENC[(*in & 0xF) << 2];
            ss << '=';
            break;
        }
        // last 4 bits of char 2, first 2 bits of char 3
        ss << ENC[((*in & 0xF) << 2) | (*(in + 1) >> 6)];
        in++;
        // last 6 bits of char 3
        ss << ENC[*in & 0x3F];
        in++, inLen--;
    }
    return ss.str();
}

std::string Base64EncodeUrlSafe(const std::string &src)
{
    return Base64EncodeUrlSafe(src.c_str(), static_cast<int>(src.size()));
}

std::string Base64EncodeUrlSafe(const char *src, int len)
{
    std::string out = Base64Encode(src, len);

    while (out.size() > 0 && *out.rbegin() == '=')
        out.pop_back();

    std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c) { 
        if (c == '+') return '-';
        if (c == '/') return '_';
        return (char)c;
    });
    return out;
}

std::string XmlEscape(const std::string& value)
{
    struct Entity {
        const char* pattern;
        char value;
    };

    static const Entity entities[] = {
        { "&quot;", '\"' },
        { "&amp;",  '&'  },
        { "&apos;", '\'' },
        { "&lt;",	'<'  },
        { "&gt;",	'>'  },
        { "&#13;",	'\r' }
    };

    if (value.empty()) {
        return value;
    }

    std::stringstream ss;
    for (size_t i = 0; i < value.size(); i++) {
        bool flag = false;
        for (size_t j = 0; j < (sizeof(entities)/sizeof(entities[0])); j++) {
            if (value[i] == entities[j].value) {
                flag = true;
                ss << entities[j].pattern;
                break;
            }
        }

        if (!flag) {
            ss << value[i];
        }
    }

    return ss.str();
}
ByteBuffer Base64Decode(const char *data, int len)
{
    int in_len = len;
    int i = 0;
    int in_ = 0;
    unsigned char part4[4];

    const int max_len = (len * 3 / 4);
    ByteBuffer ret(max_len);
    int idx = 0;

    while (in_len-- && (data[in_] != '=')) {
        unsigned char ch = data[in_++];
        if ('A' <= ch && ch <= 'Z')  ch = ch - 'A';           // A - Z
        else if ('a' <= ch && ch <= 'z') ch = ch - 'a' + 26;  // a - z
        else if ('0' <= ch && ch <= '9') ch = ch - '0' + 52;  // 0 - 9
        else if ('+' == ch) ch = 62;                          // +
        else if ('/' == ch) ch = 63;                          // /
        else if ('=' == ch) ch = 64;                          // =
        else ch = 0xff;                                       // something wrong
        part4[i++] = ch;
        if (i == 4) {
            ret[idx++] = (part4[0] << 2) + ((part4[1] & 0x30) >> 4);
            ret[idx++] = ((part4[1] & 0xf) << 4) + ((part4[2] & 0x3c) >> 2);
            ret[idx++] = ((part4[2] & 0x3) << 6) + part4[3];
            i = 0;
        }
    }

    if (i) {
        for (int j = i; j < 4; j++)
            part4[j] = 0xFF;
        ret[idx++] = (part4[0] << 2) + ((part4[1] & 0x30) >> 4);
        if (part4[2] != 0xFF) {
            ret[idx++] = ((part4[1] & 0xf) << 4) + ((part4[2] & 0x3c) >> 2);
            if (part4[3] != 0xFF) {
                ret[idx++] = ((part4[2] & 0x3) << 6) + part4[3];
            }
        }
    }

    ret.resize(idx);
    return ret;
}

ByteBuffer Base64Decode(const std::string &src)
{
    return Base64Decode(src.c_str(), src.size());
}


} // end namespace SQ