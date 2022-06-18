#include "reg2subtitle.h"
#include "json/json.h"
#include "string/utils.h"
#include "../aligner/aligner.h"
#include <vector>
#include <sstream>
#include <algorithm>
#include <iterator>
#include "libaegisub/charset_conv_win.h"

using agi::charset::ConvertW;

namespace SQ
{

typedef struct {
  std::string text;
  size_t beginTime;
  size_t endTime;
  size_t channelId;
} SentenceInfomation;

/** Converts time stamp in mts format to a string containing the time stamp for the srt format
 *
 * mts : time stamp in mimliseconds
 * srt expects a time stamp as  HH:MM:SS:MSS.
 */
std::string mts2srt(size_t ms) {
  size_t h = ms / (3600 * 1000);
  ms -= h * 3600 * 1000;
  size_t m = ms / (60 * 1000);
  ms -= m * 60 * 1000;
  size_t s = ms / 1000;
  ms %= 1000;

  enum { length = sizeof("HH:MM:SS,MSS") };
  char buf[length];
  snprintf(buf, length, "%02d:%02d:%02d,%03d", h, m, s, ms);
  return std::string(buf);
}

std::string AliReg2Srt(const std::string reg_str)
{
    Json::Value root;
    Json::Reader jsonReader;
    if (!jsonReader.parse(reg_str, root)) {
        throw std::runtime_error("Json parse failed");
    } 
    //if (root["Result"].isNull) {
    //    throw std::runtime_error("recognieze json parse failed");
    //}
    Json::Value result = root["Result"];
    if (result["Sentences"].isNull() || !result["Sentences"].isArray()) {
        throw std::runtime_error("recognieze json parse failed");
    }  
    Json::Value sentArray = result["Sentences"];
    std::vector<SentenceInfomation> sent_list;
    
    for (int nIndex = 0; nIndex < sentArray.size(); nIndex++) {
        SentenceInfomation sentInfo;
        if (sentArray[nIndex].isMember("Text") &&
            sentArray[nIndex]["Text"].isString()) {
            sentInfo.text = sentArray[nIndex]["Text"].asCString();
        }
        if (sentArray[nIndex].isMember("BeginTime") &&
            sentArray[nIndex]["BeginTime"].isInt()) {
            sentInfo.beginTime = sentArray[nIndex]["BeginTime"].asInt();
        }
        if (sentArray[nIndex].isMember("EndTime") &&
            sentArray[nIndex]["EndTime"].isInt()) {
            sentInfo.endTime = sentArray[nIndex]["EndTime"].asInt();
        }
        if (sentArray[nIndex].isMember("ChannelId") &&
            sentArray[nIndex]["ChannelId"].isInt()) {
            sentInfo.channelId = sentArray[nIndex]["ChannelId"].asInt();
        }
        //LOG_DEBUG("List Push: %s %d %d",
        //   sentInfo.text.c_str(), sentInfo.beginTime, sentInfo.endTime);

        sent_list.push_back(sentInfo);
    }

    std::stringstream ss;
    for (size_t i = 0; i < sent_list.size(); i++) { 
        if (0 != sent_list[i].channelId) continue;
        ss << i << "\n" 
            << mts2srt(sent_list[i].beginTime) << " --> " << mts2srt(sent_list[i].endTime) << "\n"
            << sent_list[i].text << "\n\n";
    }
    return std::move(ss.str());
}

void AliReg2Srt(const std::string &lang, const std::string &reg_str, SrtFile &srt_file)
{
    std::shared_ptr<RecognizeResult> reg_result = 
        std::make_shared<RecognizeResult>("ali", "", reg_str);
    std::vector<std::wstring> reg_sent_list;
    std::wstring punctuation;
    if("chinese" == SQ::ToLower(lang.c_str()) )
    {
        punctuation = ConvertW(std::string("。！？，"));
    }
    else if("english" == SQ::ToLower(lang.c_str()) )
    {
        punctuation = ConvertW(std::string("!?.,"));
    }
    else
    {
        punctuation = ConvertW(std::string("!?.,"));
    }
    
    for(const auto &piece : reg_result->GetSentPieces())
    {
        std::vector<std::wstring> sents = SQ::SplitStringW(piece.text_, 
            punctuation, 
            true
            );
        std::move(sents.begin(), sents.end(), std::back_inserter(reg_sent_list));
    }
        

    Aligner aligner(lang);
    aligner.SetRecognizeResult(reg_result);
    aligner.Align(reg_sent_list, srt_file);
}

} // end namespace SQ