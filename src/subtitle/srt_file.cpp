#include <iostream>
#include <algorithm>
#include <regex>
#include "srt_file.h"

SrtFrame::SrtFrame()
{
}

SrtFrame::SrtFrame(size_t startTime, size_t endTime, std::string text)
{
    startTime_ = startTime;
    endTime_ = endTime;
    text_ = text;
}

SrtFrame::SrtFrame(std::string startTime, std::string endTime, std::string text)
{
    startTime_ = String2Time(startTime);
    endTime_ = String2Time(endTime);
    text_ = text;
}

size_t SrtFrame::GetStartTime() const
{
    return startTime_;
}

std::string SrtFrame::GetStartTimeString() const
{
    return std::move( Time2String(startTime_) );
}

size_t SrtFrame::GetEndTime() const
{
    return endTime_;
}

std::string SrtFrame::GetEndTimeString() const
{
    return std::move( Time2String(endTime_) );
}

std::string SrtFrame::GetText() const
{
    return text_;
}

void SrtFrame::SetStartTime(size_t startTime)
{
    startTime_ = startTime;
}

void SrtFrame::SetEndTime(size_t endTime)
{
    endTime_ = endTime;
}

void SrtFrame::SetText(std::string text)
{
    text_ = text;
}

size_t SrtFrame::String2Time(std::string value)
{
    std::string szRegex = "([0-9]+):([0-9]{2}):([0-9]{2}),([0-9]{3})";
    std::regex subRegex (szRegex);
    int submatches[] = {1,2,3,4};
    std::regex_token_iterator<std::string::iterator> c ( value.begin(), value.end(), subRegex, submatches );
    std::regex_token_iterator<std::string::iterator> rend;
    std::vector<std::string> parts;
    while (c!=rend)
    {
        parts.push_back(*c++);
    }
    return atoi(parts[0].c_str()) * 3600000 + atoi(parts[1].c_str()) * 60000 + atoi(parts[2].c_str()) * 1000 + atoi(parts[3].c_str());
}

/** Converts time stamp in mts format to a string containing the time stamp for the srt format
 *
 * mts : time stamp in mimliseconds
 * srt expects a time stamp as  HH:MM:SS:MSS.
 */
std::string SrtFrame::Time2String(size_t ms) {
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

SrtFrame::~SrtFrame()
{

}

SrtFile::SrtFile()
{

}

SrtFile::~SrtFile()
{

}

std::ostream& operator<<(std::ostream& out, const SrtFile& srt_file)
{
    size_t line_index = 0;
    for ( const SrtFrame &frame : srt_file.GetFrames() ) { 
        out << line_index << "\n" 
            << frame.GetStartTimeString() << " --> " << frame.GetEndTimeString() << "\n"
            << frame.GetText() << "\n\n";
        line_index++;
    }
    return out;
}