
#include <sstream>
#include "SqClient.h"
#include "json/json.h"
#include "digest/MD5.h"

namespace SQ {
    using AlibabaNlsCommon::Outcome;
    using AlibabaNlsCommon::Error;
    using AlibabaNlsCommon::HttpClient;
    using AlibabaNlsCommon::Url;

    SqClient::SqClient()
    : CoreClient() 
    , code_(-1) 
    {

    }

    int SqClient::post(const std::string& url, 
        const std::map<std::string, std::string>& headers, 
        const std::string& body)
    {
        HttpRequest r = buildHttpRequest(url, headers, body, HttpRequest::Post);
        HttpClient::HttpResponseOutcome outcome = CoreClient::AttemptRequest(r);
        if (outcome.isSuccess())
        {
            //return JsonOutcome(outcome.result().getBody());
        }
        else
        {
            //return JsonOutcome(outcome.error());
        }
        return code_;   
    }

    int SqClient::get(std::string url)
    {
        HttpRequest r = buildHttpRequest(url, {}, "", HttpRequest::Get);
        HttpClient::HttpResponseOutcome outcome = CoreClient::AttemptRequest(r);
        if (outcome.isSuccess())
        {
            const std::string& responseBody = outcome.result().getBody();
            std::cout << "Response body:" << responseBody << std::endl;

            Json::Value json_root;
            Json::Reader json_reader;
            if (!json_reader.parse(responseBody, json_root)) {
                code_ = -1;
                msg_ = "Response json parse failed: ";
                msg_ += responseBody;
                return code_;
            }

            code_ = json_root["code"].asUInt();
            msg_ = json_root["msg"].asCString();
            if(json_root["data"].isString())
            {
                data_ = json_root["data"].asCString();
            }
            else
            {
                Json::FastWriter json_writer;
                data_ = json_writer.write(json_root["data"]);
            }
            return code_;
        }
        else
        {
            code_ = -1;
            msg_ = outcome.error().errorMessage();
            return code_;
        } 
        return code_;
    }

    HttpRequest SqClient::buildHttpRequest(const std::string& url_str, 
        const std::map<std::string, std::string>& headers, 
        const std::string& body,
        HttpRequest::Method method) const 
    {
        Url url(url_str);
        HttpRequest request(url);
    
        for (const auto &kv : headers )
        {
            request.setHeader(kv.first, kv.second);
        }
        
        if (!body.empty()) {
            request.setBody(body.c_str(), body.size());
            std::stringstream sLength;
            sLength << body.size();
            request.setHeader("Content-Length", sLength.str());
            request.setHeader("Content-Type", "application/x-www-form-urlencoded;charset=utf-8");
            request.setHeader("Content-MD5", ComputeContentMD5(body.c_str(), body.size()));
        }

        request.setMethod(method);
        return request;
    }
} // end namespace SQ
