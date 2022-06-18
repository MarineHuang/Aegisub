
#pragma once

#include <string>
#include <vector>

namespace SQ
{
    using ByteBuffer = std::vector<unsigned char>;

    std::string UrlEncode(const std::string &src);
    std::string UrlDecode(const std::string &src);

    std::string Base64Encode(const std::string &src);
    std::string Base64Encode(const ByteBuffer& buffer);
    std::string Base64Encode(const char *src, int len);
    std::string Base64EncodeUrlSafe(const std::string &src);
    std::string Base64EncodeUrlSafe(const char *src, int len);

    std::string XmlEscape(const std::string& value);

    ByteBuffer Base64Decode(const char *src, int len);
    ByteBuffer Base64Decode(const std::string &src);

} // end namespace SQ
