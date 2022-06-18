#include "gtest/gtest.h"
#include "utils.h"
#include "vector"

using namespace SQ;


TEST(encode, base64) 
{         
    std::string str = Base64Encode("www.baidu.com");
    ASSERT_STREQ(str.c_str(), "d3d3LmJhaWR1LmNvbQ==");
}

