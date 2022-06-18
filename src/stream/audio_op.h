#ifndef _AUDIO_OP_H_
#define _AUDIO_OP_H_


#include <string>
#include <type_traits>
#include <iostream>
#include "libaegisub/fs.h"
#include "libaegisub/io.h"

extern "C" {
    #include <libavutil/log.h>
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavformat/avio.h>
}


enum class AUDIO_OP_ERRORS {
    SUCCESS,   // 0， 无错误
    OPEN_FILE_ERROR, // 1, 打开文件错误
    OPEN_INPUT_VIDEO_ERROR, // 2, 打开输入的视频文件出错
    FIND_STREAM_ERROR, // 3, 查找流信息失败错误
    FIND_AUDIO_ERROR, // 4, 查找音频流失败
};

template<typename T>
std::ostream& operator<<(typename std::enable_if<std::is_enum<T>::value, std::ostream>::type& stream, const T& e)
{
    return stream << static_cast<typename std::underlying_type<T>::type>(e);
}

struct AudioOpWrapper {
    // 输出文件句柄
    agi::io::Save dst_fd;
    // 输入文件格式
    AVFormatContext* fmt_ctx;
    // 每次读取的音频包
    AVPacket pkt;

    AudioOpWrapper(const agi::fs::path &dst_file_path) 
    : dst_fd(dst_file_path, true)
    , fmt_ctx(nullptr)
    , pkt() { }

    void write(const char *buf, size_t buf_size) {
        std::ostream& out = dst_fd.Get();
        out.write(buf, buf_size);
    }

    ~AudioOpWrapper() {
        if(pkt.buf != nullptr) {
            av_packet_unref(&pkt);
        }
        if(fmt_ctx) {
            avformat_close_input(&fmt_ctx);
        }
    }
};

struct AudioOp
{
    AudioOp();
    ~AudioOp();

    void extract_audio(const agi::fs::path &src_video, 
        const agi::fs::path &dst_audio, 
        AUDIO_OP_ERRORS &err);
    
    private:
        void adts_header(char* szAdtsHeader, int dataLen);
};

#endif // _AUDIO_OP_H_