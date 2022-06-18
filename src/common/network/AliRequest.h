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

#ifndef ALIBABANLS_COMMON_ALIREQUEST_H_
#define ALIBABANLS_COMMON_ALIREQUEST_H_

#include <string>
#include <map>
#include "Url.h"
#include "HttpRequest.h"

namespace AlibabaNlsCommon {

class AliRequest {
 public:
  enum RequestPattern {
    TokenPattern,
    FileTransPattern
  };

  explicit AliRequest(RequestPattern pattern = TokenPattern);
  ~AliRequest();

  const char* content()const;
  size_t contentSize()const;
  bool hasContent()const;
  void setContent(const char *data, size_t size);

  std::string task()const;
  void setTask(const std::string &task);

  std::string taskId()const;
  void setTaskId(const std::string &taskId);

  std::string action()const;
  void setAction(const std::string &action);

  std::string domain() const;
  void setDomain(const std::string &domain);
  
  std::string version()const;
  void setVersion(const std::string &version);
  
  std::string product()const;

  std::string resourcePath()const;
  void setResourcePath(const std::string &path);

  RequestPattern requestPattern() const;
  void setRequestPattern(RequestPattern pattern);
  
  std::string headerParameter(const std::string &name) const;
  std::map<std::string, std::string> headerParameters() const;
  void setHeaderParameter(const std::string &name,
                          const std::string &value);

  std::string queryParameter(const std::string &name) const;
  std::map<std::string, std::string> queryParameters() const;
  void setQueryParameter(const std::string &name,
                         const std::string &value);
  

  HttpRequest::Method httpMethod() const;
  void setHttpMethod(HttpRequest::Method method);

 private:
  char *content_;
  size_t contentSize_;
  std::string task_;
  std::string taskId_;
  std::string action_;
  std::string domain_;
  std::string version_;
  std::string product_;
  std::string resourcePath_; 
  RequestPattern requestPattern_;

  std::map<std::string, std::string> queryParams_;
  std::map<std::string, std::string> headerParams_;
  HttpRequest::Method httpMethod_;
};

}
#endif // !ALIBABANLS_COMMON_ALIREQUEST_H_
