
#pragma once
#include "CoreClient.h"

namespace SQ {

using AlibabaNlsCommon::CoreClient;
using AlibabaNlsCommon::HttpRequest;

class SqClient : public CoreClient {
  public:
    
    SqClient();
    ~SqClient() {};

    int post(const std::string& url, 
            const std::map<std::string, std::string>& headers, 
            const std::string& body);
    
    int get(std::string url);
    
    /**
    * @brief 获取返回码.
    * @return 
    */
    int getCode() const { return code_; };
      
    /**
     * @brief 获取结果.
     * @note
     * @return 
     */
    const std::string& getData() const { return data_; };

    /**
    * @brief 获取错误信息.
    * @return 
    */
    const std::string& getMsg() const { return msg_; };

  private:
    HttpRequest buildHttpRequest(const std::string& url, 
        const std::map<std::string, std::string>& headers, 
        const std::string& body,
        HttpRequest::Method method) const ;
  
  private:
    int code_;
	std::string data_;
    std::string msg_;

};

} //end namespace SQ
