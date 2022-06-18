#include "gtest/gtest.h"
#include "json.h"
#include "utils.h"

using namespace SQ;

TEST(json, json2map) 
{
    std::string json_str = "{\"name\":\"jack\",\"sex\":\"male\"}";
    auto json_map = JsonStringToMap(json_str);
    ASSERT_STRCASEEQ(
        json_map["name"].c_str(), 
        "jack"
    );
    ASSERT_STRCASEEQ(
        json_map["sex"].c_str(), 
        "male"
    );
}
