
#pragma once

#include <string>
#include <map>

namespace SQ
{
    std::map<std::string, std::string> JsonStringToMap(const std::string& jsonStr);
    std::string MapToJsonString(const std::map<std::string, std::string>& map);
}
