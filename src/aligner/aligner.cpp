
#include "aligner.h"
#include "../subtitle/srt_file.h"
#include "libaegisub/log.h"
#include "json/json.h"
#include "string/utils.h"
#include "libaegisub/charset_conv_win.h"

extern "C" {
#include "_levenshtein.h"
}

using agi::charset::ConvertW;

RecognizeResult::RecognizeResult(const std::string &engine, 
        const std::string &task_id,
        const std::string &reg_result)
: engine_(engine)
, task_id_(task_id)
{
    if("ali" == engine_)
    {
        try{
            AliRecognizeResult(reg_result);
        } 
        catch (const std::runtime_error &e) {
            LOG_E("agi/RecognizeResult") << "initialize failed: " << e.what();
        }  
    }
    else
    {
        LOG_E("agi/RecognizeResult") << "engine not yet support: " << engine_;
    }
}

RecognizeResult::~RecognizeResult()
{

}


void RecognizeResult::AliRecognizeResult(const std::string &reg_result)
{

    SQ::Json::Value root;
    SQ::Json::Reader jsonReader;
    if (!jsonReader.parse(reg_result, root)) {
        throw std::runtime_error("Json parse failed");
    } 
    if (root["Result"].isNull()) {
        throw std::runtime_error("Json parse failed");
    }

    SQ::Json::Value result = root["Result"];
    if (result["Sentences"].isNull() || !result["Sentences"].isArray()) {
        throw std::runtime_error("Json parse failed");
    }  

    SQ::Json::Value sentArray = result["Sentences"];
    for (int nIndex = 0; nIndex < sentArray.size(); nIndex++) {
        RegPiece sentPiece;
        if (sentArray[nIndex].isMember("Text") &&
            sentArray[nIndex]["Text"].isString()) {
            sentPiece.text_ = ConvertW(sentArray[nIndex]["Text"].asCString());
        }
        if (sentArray[nIndex].isMember("BeginTime") &&
            sentArray[nIndex]["BeginTime"].isInt()) {
            sentPiece.begin_time_ = sentArray[nIndex]["BeginTime"].asInt();
        }
        if (sentArray[nIndex].isMember("EndTime") &&
            sentArray[nIndex]["EndTime"].isInt()) {
            sentPiece.end_time_ = sentArray[nIndex]["EndTime"].asInt();
        }
        if (sentArray[nIndex].isMember("ChannelId") &&
            sentArray[nIndex]["ChannelId"].isInt()) {
            sentPiece.channel_id_ = sentArray[nIndex]["ChannelId"].asInt();
        }
        sentences_.push_back(sentPiece);
    }

    if (result["Words"].isNull() || !result["Words"].isArray()) {
        throw std::runtime_error("Json parse failed");
    }  

    SQ::Json::Value wordArray = result["Words"];
    for (int nIndex = 0; nIndex < wordArray.size(); nIndex++) {
        RegPiece wordPiece;
        if (wordArray[nIndex].isMember("Word") &&
            wordArray[nIndex]["Word"].isString()) {
            wordPiece.text_ = ConvertW(wordArray[nIndex]["Word"].asCString());
        }
        if (wordArray[nIndex].isMember("BeginTime") &&
            wordArray[nIndex]["BeginTime"].isInt()) {
            wordPiece.begin_time_ = wordArray[nIndex]["BeginTime"].asInt();
        }
        if (wordArray[nIndex].isMember("EndTime") &&
            wordArray[nIndex]["EndTime"].isInt()) {
            wordPiece.end_time_ = wordArray[nIndex]["EndTime"].asInt();
        }
        if (wordArray[nIndex].isMember("ChannelId") &&
            wordArray[nIndex]["ChannelId"].isInt()) {
            wordPiece.channel_id_ = wordArray[nIndex]["ChannelId"].asInt();
        }
        words_.push_back(wordPiece);
    }
}

Aligner::Aligner(const std::string &lang)
: lang_(lang)
, reg_result_(nullptr)
{

}

Aligner::~Aligner()
{

}

void Aligner::SetRecognizeResult(std::shared_ptr<RecognizeResult> &reg)
{
    reg_result_ = reg;
}

/**  
 * @brief 将句子分割成字词
 *         中文按字分割
 *         英文按单词分割 
 * @param  std::string sent 待分割的句子
 * @return 分割后的字词列表
 * @code 
 * 
 * 
 * @endcode
 * @see 
 */
std::vector<std::wstring> Aligner::Split(std::wstring sent)
{
    std::vector<std::wstring> words;
    if("english" == SQ::ToLower(lang_.c_str()) )
    {
        std::wstring space = ConvertW(std::string(" "));
        words = SQ::SplitStringW(sent, space, true);
    }
    else if("chinese" == SQ::ToLower(lang_.c_str()) )
    {
        for(const auto &w : sent)
        {
            words.emplace_back(std::wstring(1, w));
        }
    }
    else
    {
        LOG_E("agi/Aligner") << "language not yet supported: " << lang_;
    }
    return std::move(words);
}

/**  
 * @brief  文字与音频对齐
 *         根据语音识别结果，给没有时间戳信息的句子赋予时间戳信息 
 * @param  const std::vector<std::string> &sent_list 没有时间戳信息的句子列表
 * @param  SrtFile &srt_file 返回的srt文档
 * @return 
 *     
 * @code 
 * 
 * 
 * @endcode
 * @see 
 */
void Aligner::Align(const std::vector<std::wstring> &sent_list, SrtFile &srt_file)
{
    // 1.组装reg_words_
    for(const RegPiece &reg_piece : reg_result_->GetWordPieces())
    {
        std::vector<std::wstring> words = Split(reg_piece.text_);
        size_t begin_time = reg_piece.begin_time_;
        size_t time_increase = (reg_piece.end_time_ - reg_piece.begin_time_) / words.size();
        for(int i=0; i< words.size(); i++)
        {   
            RegPiece word_piece;
            word_piece.text_ = words[i];
            word_piece.begin_time_ = begin_time + i*time_increase;
            word_piece.end_time_ = begin_time + (i+1)*time_increase;
            auto it = word_dict_.find(words[i]);
            if( it == word_dict_.end())
            {
                word_piece.word_id_ = word_dict_.size();
                word_dict_.insert(std::make_pair(words[i], word_dict_.size()));
            }
            else
            {
                word_piece.word_id_ = it->second;
            }
            reg_words_.emplace_back(word_piece);
        }
        
    }
    // 2.组装to_align_words_
    for(size_t line_no = 0; line_no < sent_list.size(); line_no++)
    {
        // 当前句子的第1个字词
        size_t first_word_id = to_align_words_.size();

        std::vector<std::wstring> this_sent_words = Split(sent_list[line_no]);
        if (this_sent_words.size() == 0) continue;
        for(const auto &word : this_sent_words)
        {
            RegPiece piece;
            piece.text_  = word;
            piece.line_idx_ = line_no;

            auto it = word_dict_.find(word);
            if( it == word_dict_.end())
            {
                piece.word_id_ = word_dict_.size();
                word_dict_.insert(std::make_pair(word, word_dict_.size()));
            }
            else
            {
                piece.word_id_ = it->second;
            }
            to_align_words_.emplace_back( std::move(piece) );
        }

        // 当前句子的第1个字词
        size_t last_word_id = to_align_words_.size() - 1;
        // todo: assert last_word_id >= first_word_id
        line2word_dict_[line_no] = std::make_pair(first_word_id, last_word_id);
    }
    // 3.利用reg_words_和to_align_words_分别生成对齐所需的字符串
    std::wstring reg_words_str;
    reg_words_str.reserve(reg_words_.size());
    for(const RegPiece &piece : reg_words_)
        reg_words_str.push_back(wchar_t(piece.word_id_));
    
    std::wstring to_align_words_str;
    to_align_words_str.reserve(to_align_words_.size());
    for(const RegPiece &piece : to_align_words_)
        to_align_words_str.push_back(wchar_t(piece.word_id_));

    // 4.利用levenshtein库获取对齐两个字符串所需的编辑操作
    size_t editop_count=0, blockop_count=0; 
    LevEditOp *edit_ops = lev_u_editops_find(reg_words_str.size(), reg_words_str.c_str(), 
                    to_align_words_str.size(), to_align_words_str.c_str(), 
                    &editop_count);
    
    if (!edit_ops && editop_count)
        LOG_E("agi/Aligner") << "levenshtein error when finding edit operations";

    LevOpCode *block_ops = lev_editops_to_opcodes(editop_count, edit_ops, &blockop_count, 
                    reg_words_str.size(), to_align_words_str.size());

    if (!block_ops && blockop_count)
        LOG_E("agi/Aligner") << "levenshtein error when finding block operations";
    
    // 5.利用编辑操作列表，赋予待对齐文本的时间戳信息
    LevOpCode *bops = block_ops;
    for(size_t i = 0; i < blockop_count; i++, bops++)
    {
        switch(bops->type)
        {
            case LEV_EDIT_KEEP:
                {
                    // equal
                    // assert bops->send - bops->sbeg  == bops->dend - bops->dbeg
                    // 'length of sequences which will be operated must be equal'
                    for(long si = bops->sbeg, di = bops->dbeg;
                        si < bops->send;
                        si++, di++)
                    {
                        to_align_words_[di].begin_time_ = reg_words_[si].begin_time_;
                        to_align_words_[di].end_time_ = reg_words_[si].end_time_;
                    }
                }
                break;
            case LEV_EDIT_REPLACE:
                {
                    // replace
                    // assert bops->send - bops->sbeg  == bops->dend - bops->dbeg
                    // 'length of sequences which will be operated must be equal'
                    for(long si = bops->sbeg, di = bops->dbeg;
                        si < bops->send;
                        si++, di++)
                    {
                        to_align_words_[di].begin_time_ = reg_words_[si].begin_time_;
                        to_align_words_[di].end_time_ = reg_words_[si].end_time_;
                    }
                }
                break;
            case LEV_EDIT_INSERT:
                {
                    // insert
                    // assert  bops->send == bops->sbeg
                    // 'insert operation must be at a single position of source sequence'
                    long si = (0 != bops->sbeg) ? (bops->sbeg - 1) : 0; // 要插入的字词的索引
                    for(long di = bops->dbeg;
                        di < bops->dend;
                        di++)
                    {
                        to_align_words_[di].begin_time_ = reg_words_[si].begin_time_;
                        to_align_words_[di].end_time_ = reg_words_[si].end_time_;
                    }
                }
                break;
            case LEV_EDIT_DELETE:
                {
                    // delete
                    // assert  bops->dend == bops->dbeg 
                    // 'delete operation must be at a single position of destination sequence'
                    if ( 0 ==bops->dbeg  )
                    {
                        LOG_I("agi/Aligner") << "delete operation occured at position 0 of destination sequence";
                        continue;
                    }
                    long di = bops->dbeg - 1; // 要删除的字词的索引
                    for(long si = bops->sbeg;
                        si < bops->send;
                        si++)
                    {
                        to_align_words_[di].end_time_ = reg_words_[si].end_time_;
                    }
                }
                break;
            default:
                LOG_E("agi/Aligner") << "invalid type of block operation";
                break;
        }

    }

    free(edit_ops);
    free(block_ops);

    // 6.生成SrtFile
    srt_file.Clear();
    for(size_t line_no=0; line_no < sent_list.size(); line_no++)
    {   
        if(line2word_dict_.find(line_no) == line2word_dict_.end()) continue;
        size_t first_word_id = line2word_dict_.at(line_no).first;
        size_t last_word_id = line2word_dict_.at(line_no).second;
        size_t begin_time = to_align_words_[first_word_id].begin_time_;
        size_t end_time = to_align_words_[last_word_id].end_time_;

        SrtFrame frame(begin_time, end_time, ConvertW(sent_list[line_no]));
        srt_file.PushBack(frame);
    }
    
}