
#pragma once
#include <string>
#include <vector>

class SrtFile;

/**  
 * @brief 语音识别结果中的词片段、句子片段, 也用于音频、文字对齐功能
 *   word_id_和line_idx_的用于详见音频、文字对齐功能
 * @code 
 * 
 * @endcode
 * @see 
 */
struct RegPiece
{
    std::wstring text_; // 片段的文本, 类型为wchar_t, 使用unicode存储文本数据
    size_t begin_time_; // 片段的开始时间，单位是mimliseconds
    size_t end_time_; // 片段的结束时间，单位是mimliseconds
    size_t channel_id_; // 声道id，有些引擎的识别结果可能没有该字段
    size_t word_id_; // 该字段用于音频、文字对齐功能，不是原始识别结果中的内容
    size_t line_idx_; // 该字段用于音频、文字对齐功能，不是原始识别结果中的内容
};

/**  
 * @brief 存储音频、视频的语音识别结果 
 *   
 * @code 
 * 
 * 
 * @endcode
 * @see 
 */
class RecognizeResult
{
public:
    RecognizeResult(const std::string &engine, 
        const std::string &task_id,
        const std::string &reg_result);
    ~RecognizeResult();

    const std::vector<RegPiece>& GetWordPieces() const { return words_; }
    const std::vector<RegPiece>& GetSentPieces() const { return sentences_; }
private:
    void AliRecognizeResult(const std::string &reg_result);
    std::vector<RegPiece> words_;
    std::vector<RegPiece> sentences_;
    std::string engine_;
    std::string task_id_;
};

/**  
 * @brief 实现音频、文字对齐功能 
 *   
 * @code 
 * 
 * 
 * @endcode
 * @see 
 */
class Aligner 
{
public:
    Aligner(const std::string &lang);
    ~Aligner();
    void SetRecognizeResult(std::shared_ptr<RecognizeResult> &reg);
    void Align(const std::vector<std::wstring> &sent_list, SrtFile &srt_file);

private:
    std::vector<std::wstring> Split(std::wstring sent);

private:
    // 语言
    std::string lang_;
    // 语音识别结果
    std::shared_ptr<RecognizeResult> reg_result_;
    // 语音识别结果中的词信息
    std::vector<RegPiece> reg_words_;
    // 待对齐文本中的词信息
    std::vector<RegPiece> to_align_words_;
    // 将word映射为word_id的词典
    std::map<std::wstring, size_t> word_dict_;
    // 存储待对齐文本的句子信息
    // line_index <=> (first_word_id, last_word_id)
    std::map<size_t, std::pair<size_t, size_t>> line2word_dict_;
};
