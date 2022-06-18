#include "gtest/gtest.h"
#include "MD5.h"

TEST(digest, MD5) 
{
    // 忽略大小写
    ASSERT_STRCASEEQ(
        SQ::ComputeContentMD5("sqsanshao").c_str(), 
        "9A73E9B1F7A09B2347FB13C0DA2121FD"
    );
    ASSERT_STRCASEEQ(
        SQ::ComputeContentMD5("this is a test for MD5").c_str(), 
        "2e968e9e37e76213fdcd67c849eeb860"
    );
}
// TODO: test crc32 and crc64