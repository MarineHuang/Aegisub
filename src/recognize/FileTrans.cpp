/*
 * Copyright 2009-2017 Alibaba Cloud All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <iostream>
#include <string.h>
#include "network/AliClient.h"
#include "json/json.h"
#include "FileTrans.h"

namespace AlibabaNlsCommon {

using namespace SQ;

FileTrans::FileTrans() {
  accessKeySecret_ = "";
  accessKeyId_ = "";
  appKey_ = "";
  fileLink_ = "";
  errorMsg_ = "";
  result_ = "";
  regionId_ = "";
  endpointName_ = "";
  //serverResourcePath_ = "";

  domain_ = "filetrans.cn-shanghai.aliyuncs.com";
  serverVersion_ = "2018-08-17";
  product_ = "nls-filetrans";

  taskId_ = "9e1573aaf61b46fcbe36bd1acbb5fb39";
}

FileTrans::~FileTrans() {
}

int FileTrans::paramCheck() {
  if (accessKeySecret_.empty()) {
    errorMsg_ = "AccessKeySecret is empty.";
    return -1;
  }

  if (accessKeyId_.empty()) {
    errorMsg_ = "AccessKeyId is empty.";
    return -1;
  }

  if (appKey_.empty()) {
    errorMsg_ = "AppKey is empty.";
    return -1;
  }

  if (fileLink_.empty()) {
    errorMsg_ = "FileLink is empty.";
    return -1;
  }

  if (domain_.empty()) {
    errorMsg_ = "Domain is empty.";
    return -1;
  }

  if (serverVersion_.empty()) {
    errorMsg_ = "ServerVersion is empty.";
    return -1;
  }


  return 0;
}

int FileTrans::applyFileTrans() {
  int retCode = 0;
  
  Json::Value root;
  Json::Reader reader;
  //Json::Value::iterator iter;
  //Json::Value::Members members;
  Json::FastWriter writer;

  if (paramCheck() == -1) {
    return -1;
  }

  if (!customParam_.empty()) {
    if (!reader.parse(customParam_.c_str(), root)) {
      return -1;
    }

    if (!root.isObject()) {
      return -1;
    }
  }
  root["app_key"] = appKey_.c_str();
  root["file_link"] = fileLink_.c_str();

  //ClientConfiguration默认区域id为hangzhou
  ClientConfiguration configuration("cn-shanghai");
  if (!regionId_.empty()) {
    configuration.setRegionId(regionId_);
  }

  AliClient client(accessKeyId_, accessKeySecret_, configuration);

  AliRequest taskRequest(AliRequest::FileTransPattern);
  taskRequest.setDomain(domain_);
  taskRequest.setVersion(serverVersion_);
  taskRequest.setHttpMethod(HttpRequest::Post);
  taskRequest.setAction("SubmitTask");
  std::string taskContent = writer.write(root);
  //std::cout << "Output: " << taskContent << std::endl;
  taskRequest.setTask(taskContent);

  AliClient::JsonOutcome outcome = client.AttemptRequest(taskRequest);
  if (!outcome.isSuccess()) {
    // 异常处理
    errorMsg_ = outcome.error().errorMessage();
    return -1;
  }

  Json::Value requestJson;
  Json::Reader requestReader;
  const std::string& requestString = outcome.result();
  //std::cout << "Request:" << requestString << std::endl;

  if (!requestReader.parse(requestString, requestJson)) {
    errorMsg_ = "Json parse failed: ";
    errorMsg_ += requestString;
    return -1;
  }

  if (!requestJson["StatusCode"].isNull()) {
    Json::Value::UInt statusCode = requestJson["StatusCode"].asUInt();
    if ((statusCode != 21050000) && (statusCode != 21050003)) {
      errorMsg_ = requestJson["StatusText"].asCString();
      return -1;
    }
  } else {
    errorMsg_ = "Json parse failed.";
    return -1;
  }

  taskId_ = requestJson["TaskId"].asCString();
  std::cout << "taskId:" << taskId_ << std::endl;
  return retCode;
}

int FileTrans::applyTransResult() {
  int retCode = 0;

  //ClientConfiguration默认区域id为hangzhou
  ClientConfiguration configuration("cn-shanghai");
  if (!regionId_.empty()) {
    configuration.setRegionId(regionId_);
  }
  //NetworkProxy proxy(NetworkProxy::Http, "127.0.0.1", 8888);
  //configuration.setProxy(proxy);

  AliClient client(accessKeyId_, accessKeySecret_, configuration);
  
  AliRequest resultRequest(AliRequest::FileTransPattern);
  resultRequest.setDomain(domain_);
  resultRequest.setVersion(serverVersion_);
  resultRequest.setHttpMethod(HttpRequest::Get);
  resultRequest.setAction("GetTaskResult");
  resultRequest.setTaskId(taskId_);

  Json::Value resultJson;
  Json::Reader resultReader;
  Json::Value::UInt statusCode;

  AliClient::JsonOutcome resultOutcome;

  do {
    resultOutcome = client.AttemptRequest(resultRequest);
    if (!resultOutcome.isSuccess()) {
      // 异常处理
      errorMsg_ = resultOutcome.error().errorMessage();
      retCode = -1;
      break;
    }

    const std::string& resultString = resultOutcome.result();
    //std::cout << "resultString size:" << resultString.size() << std::endl;
    //std::cout << "resultString:" << resultString << std::endl;

    resultJson.clear();
    if (!resultReader.parse(resultString, resultJson)) {
      errorMsg_ = "Json parse failed: ";
      errorMsg_ += std::move(resultString);
      retCode = -1;
      break;
    }

    if (!resultJson["StatusCode"].isNull()) {
      statusCode = resultJson["StatusCode"].asUInt();
      if ((statusCode == 21050001) || (statusCode == 21050002)) {

#if defined(_WIN32)
        Sleep(100);
#else
        usleep(100 * 1000);
#endif
      } else if ((statusCode == 21050000) || (statusCode == 21050003)){
        result_ = std::move(resultString);
        break;
      } else {
        errorMsg_ = resultJson["StatusText"].asCString();
        retCode = -1;
        break;
      }
    } else {
      errorMsg_ = std::move(resultString);
      retCode = -1;
      break;
    }
    //std::cout << "statusCode:" << statusCode << std::endl;
  } while((statusCode == 21050001) || (statusCode == 21050002));

  return retCode;
}

const char* FileTrans::getErrorMsg() {
  return errorMsg_.c_str();
}

std::string& FileTrans::getResult() {
  return result_;
}

void FileTrans::setKeySecret(const std::string & KeySecret) {
  accessKeySecret_ = KeySecret;
}

void FileTrans::setAccessKeyId(const std::string & accessKeyId) {
  accessKeyId_ = accessKeyId;
}

void FileTrans::setDomain(const std::string & domain) {
  domain_ = domain;
}

void FileTrans::setServerVersion(const std::string & serverVersion) {
  serverVersion_ = serverVersion;
}

void FileTrans::setAppKey(const std::string & appKey) {
  appKey_ = appKey;
}

void FileTrans::setFileLinkUrl(const std::string & fileLinkUrl) {
  fileLink_ = fileLinkUrl;
}

void FileTrans::setRegionId(const std::string & regionId) {
  regionId_ = regionId;
}


void FileTrans::setCustomParam(const std::string & customJsonString) {
  customParam_ = customJsonString;
}

const std::string& FileTrans::getTaskId() {
  return taskId_;
}

void FileTrans::setTaskId(const std::string & taskId) {
  taskId_ = taskId;
}

}
