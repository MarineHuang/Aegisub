
#include "utils.h"
#include "json.h"
namespace SQ
{

std::map<std::string, std::string> JsonStringToMap(const std::string& jsonStr)
{
    std::map<std::string, std::string> valueMap;
    Json::Value root;
    Json::CharReaderBuilder rbuilder;
    std::stringstream input(jsonStr);
    std::string errMsg;

    if (Json::parseFromStream(rbuilder, input, &root, &errMsg)) {

        for (auto it = root.begin(); it != root.end(); ++it)
        {
            valueMap[it.key().asString()] = (*it).asString();
        }
    }

    return valueMap;
}

std::string MapToJsonString(const std::map<std::string, std::string>& map)
{
    if (map.empty()) {
        return "";
    }
    Json::Value root;
    for (const auto& it : map) {
        root[it.first] = it.second;
    }

    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    return Json::writeString(builder, root);
}


} // end namespace SQ
