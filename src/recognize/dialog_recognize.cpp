
#include "dialog_recognize.h"
#include "../subs_controller.h"
#include "../include/aegisub/context.h"
#include "../text_file_writer.h"
#include "../ass_file.h"
#include "../dialog_progress.h"
#include "../subtitle/srt_file.h"
#include "libaegisub/log.h"
#include "libaegisub/path.h"
#include "../libresrc/libresrc.h"
#include "libaegisub/fs.h"
#include "libaegisub/io.h"
#include "../project.h"
#include "../options.h"
#include "alibabacloud/oss/OssClient.h"
#include "NlsToken.h"
#include "FileTrans.h"
#include "encode/charset.h"
#include "reg2subtitle.h"
#include "../stream/audio_op.h"
#include "network/SqClient.h"
#include "encode/utils.h"
#include "json/utils.h"
#include <wx/sizer.h>
#include <wx/msgdlg.h>


#define CHECK_MAP_STRING(map, key_string)                                                       \
    do{                                                                                         \
        auto it = map.find(key_string);                                                         \
        if(map.end() == it || it->second.empty())                                               \
        {                                                                                       \
            LOG_E("agi/recognize")  << "invalid configurate because key: " << key_string;       \
            return -1;                                                                          \
        }                                                                                       \
    }while(0);

// oss配置
static	std::string gOssBucketName("shanghai-audio-bucket");
static	std::string gOssEndpointDomain("oss-cn-shanghai.aliyuncs.com");
static  std::string gOssInternalEndpointDomain("oss-cn-shanghai-internal.aliyuncs.com");

/**  
 * @brief 获取oss sts token
 * @param std::map<std::string, std::string> &config 获取的sts token参数
 * @return 
 *     0    success 
 *     -1   failed  
 */
static int GetOssToken(std::map<std::string, std::string> &config)
{
    std::string url("http://121.41.167.142:8080/oss/token");
    SQ::SqClient sqClient;
    int code = sqClient.get(url);
    if (-1 == code )
    {
        LOG_E("agi/recognize") << "GetOssToken failed";
		LOG_E("agi/recognize") << "code:" << sqClient.getCode();
		LOG_E("agi/recognize") << "message:" << sqClient.getMsg();
        return -1;
    }
    else
    {
        std::string dataStr = sqClient.getData();
        config = SQ::JsonStringToMap(dataStr);
        return 0;
    }
}

/**  
 * @brief 上传本地文件至oss
 * @param local_path 本地文件的路径 
 * @param oss_path 目标路径
 * @return 
 *     0    success 
 *     -1   failed  
 */
static int UpLoad(const std::map<std::string, std::string> &config,
    const agi::fs::path &upload_file, const std::string &oss_object)
{
	// 参数合法性检查
    CHECK_MAP_STRING(config, "accessKeyId")
    CHECK_MAP_STRING(config, "accessKeySecret")
    CHECK_MAP_STRING(config, "securityToken")
    CHECK_MAP_STRING(config, "expiration")
    
    using namespace AlibabaCloud::OSS;
    // 上传文件
    InitializeSdk();

	ClientConfiguration conf;
	OssClient client(gOssEndpointDomain, 
        config.at("accessKeyId"), 
        config.at("accessKeySecret"), 
        config.at("securityToken"), 
        conf);
	
    std::shared_ptr<std::iostream> upload_content;
    try{
        upload_content = agi::io::Open_IO(upload_file);
    }
    catch(const agi::io::IOFatal &e)
    {
        LOG_E("agi/recognize") << "open file failed" << upload_file.string();
		ShutdownSdk();  // 释放网络等资源
        return -1;
    }
    
    PutObjectRequest request(gOssBucketName, oss_object, upload_content);

    //（可选）请参见如下示例设置访问权限ACL为私有（private）以及存储类型为标准存储（Standard）
    //request.MetaData().addHeader("x-oss-object-acl", "private");
    //request.MetaData().addHeader("x-oss-storage-class", "Standard");

    auto outcome = client.PutObject(request);
    //auto outcome = client.PutObject(gOssBucketName, oss_path, local_path);

	if (!outcome.isSuccess()) {
		// 异常处理 
		LOG_E("agi/recognize") << "PutObject fail";
		LOG_E("agi/recognize") << "code:" << outcome.error().Code();
		LOG_E("agi/recognize") << "message:" << outcome.error().Message();
		LOG_E("agi/recognize") << "requestId:" << outcome.error().RequestId();
		ShutdownSdk();  // 释放网络等资源
        return -1;
	}
	else {
		LOG_D("agi/recognize") << "PutObject success";
        ShutdownSdk(); // 释放网络等资源
        return 0;
	}
}

/**  
 * @brief 提交语音识别任务 
 * @param  audio_path 音频路径.  对于ali引擎，该路径必须是可访问的
 * @param  task_id 任务ID
 * @return 
 *     0    success 
 *    -1   failed   
 */
static int ApplyFileTrans(const std::map<std::string, std::string> &config,
    const std::string &audio_path, std::string &task_id)
{	
	// 参数合法性检查
    CHECK_MAP_STRING(config, "Access Key Id")
    CHECK_MAP_STRING(config, "Access Key Secret")
    CHECK_MAP_STRING(config, "App Key")

	AlibabaNlsCommon::FileTrans fileTransRequest;
    /*设置阿里云账号KeyId*/
    fileTransRequest.setAccessKeyId(config.at("Access Key Id"));
    /*设置阿里云账号KeySecret*/
    fileTransRequest.setKeySecret(config.at("Access Key Secret"));
    /*设置阿里云AppKey*/
    fileTransRequest.setAppKey(config.at("App Key"));
    /*设置音频文件url地址*/
    fileTransRequest.setFileLinkUrl(audio_path);

    //fileTransRequest.setCustomParam("{\"auto_split\":true,\"version\": \"4.0\",\"enable_words\": true}");
	fileTransRequest.setCustomParam("{\"version\": \"4.0\",\"enable_words\": true, \"enable_sample_rate_adaptive\": true}");

    /*开始文件转写, 成功返回0, 失败返回-1*/
    if (-1 == fileTransRequest.applyFileTrans()) {
        LOG_D("agi/recognize") <<  "Failed: " << std::string(fileTransRequest.getErrorMsg());
        return -1;
    } else {
		LOG_D("agi/recognize") << "recognize successed";
        task_id = fileTransRequest.getTaskId();
        LOG_D("agi/recognize") << "task id: " << task_id;
        return 0;
    }
}

/**  
 * @brief 查询并获取识别结果
 * @param  task_id 任务ID
 * @param  reg_result 识别结果
 * @return 
 *     0    success 
 *    -1   failed 
 */
static int GetTransResult(const std::map<std::string, std::string> &config,
    const std::string &task_id, std::string &reg_result)
{	
    // 参数合法性检查
    CHECK_MAP_STRING(config, "Access Key Id")
    CHECK_MAP_STRING(config, "Access Key Secret")
    CHECK_MAP_STRING(config, "App Key")

	AlibabaNlsCommon::FileTrans fileTransRequest;
    /*设置阿里云账号KeyId*/
    fileTransRequest.setAccessKeyId(config.at("Access Key Id"));
    /*设置阿里云账号KeySecret*/
    fileTransRequest.setKeySecret(config.at("Access Key Secret"));
    /*设置阿里云AppKey*/
    fileTransRequest.setAppKey(config.at("App Key"));
    /*设置task id*/
    fileTransRequest.setTaskId(task_id);

    // 成功返回0, 失败返回-1
    if (-1 == fileTransRequest.applyTransResult()) {
        LOG_D("agi/recognize")  << "apply trans result failed: " <<std::string(fileTransRequest.getErrorMsg());
        return -1;
    } else {
		LOG_D("agi/recognize")<< "apply trans result successed";
        reg_result = std::move((fileTransRequest.getResult()));
        return 0;
    }
}

static int GetTokenId()
{
	AlibabaNlsCommon::NlsToken nlsTokenRequest;

    /*设置阿里云账号KeySecret*/
    //nlsTokenRequest.setKeySecret(gApiAccessKeySecret);
    /*设置阿里云账号KeyId*/
    //nlsTokenRequest.setAccessKeyId(gApiAccessKeyId);

    /*获取token. 成功返回0, 失败返回-1*/
    if (-1 == nlsTokenRequest.applyNlsToken()) {
        /*获取失败原因*/
		//qDebug() << "Failed: " << QString::fromStdString(std::string(nlsTokenRequest.getErrorMsg())); 
    } else {
		/*获取TokenId*/
        //qDebug() << "TokenId: " << QString::fromStdString(std::string(nlsTokenRequest.getToken())); 
		/*获取Token有效期时间戳(秒)*/
        //qDebug() << "TokenId expireTime: " << QString::fromStdString(std::to_string(nlsTokenRequest.getExpireTime())); 
        //m_apiTokenId = nlsTokenRequest.getToken();
    }
    return 0;
}

static int SpeechRecognize(const std::map<std::string, std::string> &config,
    agi::ProgressSink *ps, 
    agi::Context *context)
{
    // 参数合法性检查
    CHECK_MAP_STRING(config, "Language")
    CHECK_MAP_STRING(config, "Access Key Id")
    CHECK_MAP_STRING(config, "Access Key Secret")
    CHECK_MAP_STRING(config, "App Key")
    agi::fs::path audio_path = context->path->MakeAbsolute(context->project->AudioName(), "?script");
    if (audio_path.empty()) {
        LOG_E("agi/recognize") << "audio file is not exist";
        return -1;
    }

    // 提取视频中的音频
    agi::fs::path src_video_path = context->path->MakeAbsolute(context->project->VideoName(), "?script");
    if(!src_video_path.empty() && src_video_path == audio_path) 
    {
        // 这是一个视频
        agi::fs::path dst_audio_path = src_video_path.parent_path();
        dst_audio_path = dst_audio_path / ( src_video_path.stem().string() + ".audio" );

        AudioOp audio_op;
        AUDIO_OP_ERRORS err = AUDIO_OP_ERRORS::SUCCESS;
        audio_op.extract_audio(src_video_path, dst_audio_path, err);
        if(err != AUDIO_OP_ERRORS::SUCCESS) {
            LOG_E("agi/recognize") << "extract audio error: " << err;
            return -1;
        }

        audio_path = dst_audio_path;
    }
    

    // get oss token
    if (ps->IsCancelled()) return -1;
    ps->SetMessage( "Geting oss token ...");
    std::map<std::string, std::string> oss_token;
    if(0 != GetOssToken(oss_token)) {
        LOG_E("agi/recognize") << "get oss token failed";
        return -1;
    }

    std::string oss_object = std::string("audio/") 
                        + audio_path.stem().string() 
                        + audio_path.extension().string();
    //std::string oss_object = std::string("audio/123456.audio");
    LOG_D("agi/recognize") << "upload audio path:" << audio_path.string();
    LOG_D("agi/recognize") << "oss object:" << oss_object;

    // 上传音频文件
    if (ps->IsCancelled()) return -1;
    ps->SetMessage( "Uploading audio ...");
    //ps->SetProgress(**,**);
    if(0 != UpLoad(oss_token, audio_path, oss_object)) {
        LOG_E("agi/recognize") << "upload audio file failed";
        return -1;
    }

    std::string oss_audio_url = "https://" + gOssBucketName + "." + gOssInternalEndpointDomain;
	oss_audio_url += "/";
	oss_audio_url += oss_object;
    LOG_D("agi/recognize") << "oss audio url:" << oss_audio_url;

    // 提交语音识别任务
    if (ps->IsCancelled()) return -1;
    ps->SetMessage( "Applying file trans..." );
    std::string task_id;
    if(0 != ApplyFileTrans(config, oss_audio_url, task_id)) {
        LOG_E("agi/recognize") << "apply file trans failed";
        return -1;
    }

    // 获取识别结果, 内部会轮询
    if (ps->IsCancelled()) return -1;
    ps->SetMessage( "Waiting trans result ..." );
    std::string reg_result;
    if(0 != GetTransResult(config, task_id, reg_result)) {
        LOG_E("agi/recognize") << "get file trans result failed";
        return -1;
    }
    
    // 将识别结果存储到本地
    if (ps->IsCancelled()) return -1;
    ps->SetMessage( "Parsing trans result ..." );
    SrtFile srt_file;
    SQ::AliReg2Srt(config.at("Language"), reg_result, srt_file);
    
    // 将识别结果写入srt文件
    agi::fs::path srt_file_path(audio_path.branch_path());
    srt_file_path /= audio_path.stem().string() + ".srt";
    LOG_D("agi/recognize") << "srt file path:" << srt_file_path;
    {
        std::ostringstream ostr;
        ostr << srt_file;
        TextFileWriter srt_file_writer(srt_file_path, "utf-8");
        srt_file_writer.WriteLineToFile(ostr.str());
    }
    // 从srt文件读取字幕到当前项目的ass对象中
    context->subsController->Load(srt_file_path, "utf-8");
}


RecognizeDialog::RecognizeDialog(agi::Context *context)
: wxDialog(context->parent, -1, _("Speech Recognize"))
, context_(context)
{ 
    SetIcon(GETICON(style_toolbutton_16));

    auto add_with_label = [&](wxSizer * sizer, wxString const& label, wxWindow * ctrl) {
        sizer->Add(new wxStaticText(this, -1, label), 0, wxLEFT | wxRIGHT | wxALIGN_RIGHT, 3);
        sizer->Add(ctrl, 1, wxLEFT | wxALIGN_LEFT);
    };

    // engine
    wxSizer *engine_sizer = new wxStaticBoxSizer(wxHORIZONTAL,this,_("Please choose auto-speech-recognizer engine"));
    engine_combo_ = new wxComboBox(this,-1, "", wxDefaultPosition, wxSize(-1,-1), 0, nullptr, wxCB_READONLY);
    //wxButton *new_engine_btn = new wxButton(this, -1, _("New"));
    engine_sizer->Add(engine_combo_,1,wxEXPAND | wxRIGHT, 5);
    //engine_sizer->Add(new_engine_btn, 0, wxLEFT, 5);

    // parameter
    wxFlexGridSizer* param_sizer = new wxFlexGridSizer(3, 2, 10, 5);   
    access_key_id_ = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxSize(300, -1));
    add_with_label(param_sizer, _("Access Key Id:"), access_key_id_);

    access_key_secret_ = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxSize(300, -1));
    add_with_label(param_sizer, _("Access Key Secret:"), access_key_secret_);

    app_key_ = new wxTextCtrl(this, -1, "", wxDefaultPosition, wxSize(300, -1));
    add_with_label(param_sizer, _("App Key:"), app_key_);
    param_sizer->AddGrowableCol(1, 1);

    wxSizer *param_static_sizer = new wxStaticBoxSizer(wxHORIZONTAL, this, _("Engine paremeter"));
    param_static_sizer->Add(param_sizer, 0, wxEXPAND);

    // buttons
    wxStdDialogButtonSizer *button_sizer = CreateStdDialogButtonSizer(wxCANCEL | wxOK);
    button_sizer->GetCancelButton()->SetLabel(_("Cancel"));
    button_sizer->GetAffirmativeButton()->SetLabel(_("Recognize"));
    
    // bind
    engine_combo_->Bind(
        wxEVT_COMBOBOX,
        (wxObjectEventFunction)&RecognizeDialog::OnComboChange,
        this
    );
    button_sizer->GetAffirmativeButton()->Bind(
        wxEVT_COMMAND_BUTTON_CLICKED,
        (wxObjectEventFunction)&RecognizeDialog::OnRecognizeButtonClick, 
        this
    );

    wxSizer *main_sizer = new wxBoxSizer(wxVERTICAL);
    main_sizer->Add(engine_sizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM | wxTOP, 10);
    main_sizer->Add(param_static_sizer, 0, wxEXPAND | wxLEFT | wxRIGHT | wxBOTTOM, 10);
    main_sizer->Add(button_sizer, 0, wxEXPAND | wxBOTTOM, 10);

    SetSizerAndFit(main_sizer);

    // 读取引擎配置
    engine_list_ = OPT_GET("Speech Recognizer/Engine List")->GetListString();
    // 设置初始值
    for(size_t i=0; i < engine_list_.size(); i++) 
    {
        engine_combo_->Insert(_( engine_list_[i] ), i);
    }
    if(!engine_combo_->IsListEmpty())
    {
        engine_combo_->SetSelection(0);
        previous_engine_ = std::string(engine_combo_->GetValue());

        std::string option_name;
        option_name = "Speech Recognizer/" + previous_engine_ + "/Access Key Id";
        access_key_id_->SetValue(_( OPT_GET(option_name.c_str())->GetString() ));

        option_name = "Speech Recognizer/" + previous_engine_ + "/Access Key Secret";
        access_key_secret_->SetValue(_( OPT_GET(option_name.c_str())->GetString() ));

        option_name = "Speech Recognizer/" + previous_engine_ + "/App Key";
        app_key_->SetValue(_( OPT_GET(option_name.c_str())->GetString() ));
    }
    
}


void RecognizeDialog::OnRecognizeButtonClick(wxCommandEvent& event)
{
    
    const std::string selected_engine = std::string(engine_combo_->GetValue());
    if(selected_engine.empty())
    {
        // Inform user to select a engine.
        wxMessageBox(_("Please select a engine!"));
        return;
    }
    else{
        // 识别前保存当前的引擎设置
        std::string option_name;
        option_name = "Speech Recognizer/" + selected_engine + "/Access Key Id";
        OPT_SET(option_name.c_str())->SetString( std::string(access_key_id_->GetValue()) );
        
        option_name = "Speech Recognizer/" + selected_engine + "/Access Key Secret";
        OPT_SET(option_name.c_str())->SetString( std::string(access_key_secret_->GetValue()) );

        option_name = "Speech Recognizer/" + selected_engine + "/App Key";
        OPT_SET(option_name.c_str())->SetString( std::string(app_key_->GetValue()) );
        // 关闭对话框
        EndModal(0);
        // 开始识别
        std::map<std::string, std::string> engine;
        engine["Language"] = selected_engine.substr(selected_engine.find_first_of(" ") + 1);
        engine["Access Key Id"] = std::string(access_key_id_->GetValue());
        engine["Access Key Secret"] = std::string(access_key_secret_->GetValue());
        engine["App Key"] = std::string(app_key_->GetValue());
        DialogProgress progress(context_->parent, _("Speech Recognize"), _("Parsing audio information..."));
        progress.Run([&](agi::ProgressSink *ps) { SpeechRecognize(engine, ps, context_); });
    }
}

void RecognizeDialog::OnComboChange(wxCommandEvent& event)
{
    std::string option_name;
    // 保存之前的设置
    if(!previous_engine_.empty())
    {
        option_name = "Speech Recognizer/" + previous_engine_ + "/Access Key Id";
        OPT_SET(option_name.c_str())->SetString( std::string(access_key_id_->GetValue()) );
        
        option_name = "Speech Recognizer/" + previous_engine_ + "/Access Key Secret";
        OPT_SET(option_name.c_str())->SetString( std::string(access_key_secret_->GetValue()) );

        option_name = "Speech Recognizer/" + previous_engine_ + "/App Key";
        OPT_SET(option_name.c_str())->SetString( std::string(app_key_->GetValue()) );
    }
    // 更新当前item的设置
    previous_engine_ = std::string(engine_combo_->GetValue());

    option_name = "Speech Recognizer/" + previous_engine_ + "/Access Key Id";
    access_key_id_->SetValue(_( OPT_GET(option_name.c_str())->GetString() ));

    option_name = "Speech Recognizer/" + previous_engine_ + "/Access Key Secret";
    access_key_secret_->SetValue(_( OPT_GET(option_name.c_str())->GetString() ));

    option_name = "Speech Recognizer/" + previous_engine_ + "/App Key";
    app_key_->SetValue(_( OPT_GET(option_name.c_str())->GetString() ));
}