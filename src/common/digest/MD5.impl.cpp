#include <sstream>
#include "MD5.h"
#include "md5.impl.h"

namespace SQ
{

std::string HexToString(const unsigned char *data, size_t size)
{ 
    static char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    std::stringstream ss;
    for (size_t i = 0; i < size; i++)
        ss << hex[(data[i] >> 4)] << hex[(data[i] & 0x0F)];
    return ss.str();
}

std::string ComputeContentMD5(const std::string& data) 
{
    return ComputeContentMD5(data.c_str(), data.size());
}

std::string ComputeContentMD5(const char * data, size_t size)
{
    if (!data || !size) {
        return "";
    }

    MD5_CTX mdContext;
    MD5Init(&mdContext);
    MD5Update(&mdContext, (unsigned char *)data, size);
    MD5Final(&mdContext);
    
    unsigned char md[DIGEST_LEN];
    memcpy(md, mdContext.digest, DIGEST_LEN);
    return HexToString(md, DIGEST_LEN);
}

std::string ComputeContentMD5(std::istream& stream) 
{
    MD5_CTX mdContext;
    MD5Init (&mdContext);

    auto currentPos = stream.tellg();
    if (currentPos == static_cast<std::streampos>(-1)) {
        currentPos = 0;
        stream.clear();
    }
    stream.seekg(0, stream.beg);

    char streamBuffer[2048];
    while (stream.good())
    {
        stream.read(streamBuffer, 2048);
        auto bytesRead = stream.gcount();

        if (bytesRead > 0)
        {
            MD5Update(&mdContext, (unsigned char *)streamBuffer, static_cast<size_t>(bytesRead));
        }
    }
    MD5Final (&mdContext);
    
    unsigned char md[DIGEST_LEN];
    memcpy(md, mdContext.digest, DIGEST_LEN);
    return HexToString(md, DIGEST_LEN);
}

} // end namespace SQ