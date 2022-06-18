#pragma once
#include <vector>

class SrtFrame
{
private:
    size_t startTime_; // in milliseconds
    size_t endTime_;
    std::string text_; // utf-8 
    
public:
    size_t GetStartTime() const;
    std::string GetStartTimeString() const;
    size_t GetEndTime() const;
    std::string GetEndTimeString() const;
    std::string GetText() const;

    void SetStartTime(size_t startTime);
    void SetEndTime(size_t endTime);
    void SetText(std::string text);

    SrtFrame();
    SrtFrame(size_t startTime, size_t endTime, std::string text);
    SrtFrame(std::string startTime,std::string endTime, std::string text);
    ~SrtFrame();

    static size_t String2Time(std::string value);
    static std::string Time2String(size_t time);
};

class SrtFile
{
public:
    SrtFile();
    //SrtFile(const SrtFile &from);
    //SrtFile& operator=(SrtFile from);
    ~SrtFile();
    const std::vector<SrtFrame>& GetFrames() const { return frames_; }
    void PushBack(SrtFrame frame) { frames_.push_back(frame); }
    void Clear() { frames_.clear(); }
private:
    std::vector<SrtFrame>  frames_;
};
// 重载<<操作符
std::ostream& operator<<(std::ostream& out, const SrtFile& srt_file);