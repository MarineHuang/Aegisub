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

#include "AliRequest.h"

namespace AlibabaNlsCommon {

AliRequest::AliRequest(RequestPattern pattern)
: requestPattern_(pattern)
, content_(NULL)
, contentSize_(0)
, resourcePath_("/")
, httpMethod_(HttpRequest::Get) {

}

AliRequest::~AliRequest() {
  
}

const char * AliRequest::content() const {
  return content_;
}

size_t AliRequest::contentSize() const {
  return contentSize_;
}

bool AliRequest::hasContent() const {
  return (contentSize_ != 0);
}

void AliRequest::setContent(const char * data, size_t size) {
  if (content_)
    delete content_;

  content_ = NULL;
  contentSize_ = 0;

  if (size) {
    contentSize_ = size;
    content_ = new char[size];
    memset(content_, 0x0, contentSize_);
    memcpy(content_, data, size);
    //std::copy(data, data + size, content_);
  }
}

AliRequest::RequestPattern AliRequest::requestPattern() const {
  return requestPattern_;
}

void AliRequest::setRequestPattern(RequestPattern pattern) {
  requestPattern_ = pattern;
}


std::string AliRequest::queryParameter(const std::string &name)const {
  return queryParams_.at(name);
}

std::map<std::string, std::string> AliRequest::queryParameters() const {
  return queryParams_;
}

void AliRequest::setQueryParameter(const std::string &name, const std::string &value) {
  queryParams_[name] = value;
}
 
std::string AliRequest::headerParameter(const std::string &name) const {
  return headerParams_.at(name);
}

std::map<std::string, std::string> AliRequest::headerParameters() const {
  return headerParams_;
}

void AliRequest::setHeaderParameter(
    const std::string &name, const std::string &value) {
  headerParams_[name] = value;
}


std::string AliRequest::task()const {
  return task_;
}

void AliRequest::setTask(const std::string &task) {
  task_ =  task;

  std::string tmp = "Task=";
  tmp += task;
  setContent(tmp.c_str(), tmp.size());
}

std::string AliRequest::taskId()const {
  return taskId_;
}

void AliRequest::setTaskId(const std::string &taskId) {
  taskId_ = taskId;
}

std::string AliRequest::action() const {
  return action_;
}

void AliRequest::setAction(const std::string &action) {
    action_ = action;
}

std::string AliRequest::domain()const {
  return domain_;
}

void AliRequest::setDomain(const std::string &domain) {
  domain_ = domain;
}

std::string AliRequest::version()const {
  return version_;
}

void AliRequest::setVersion(const std::string &version) {
  version_ = version;
}

std::string AliRequest::product() const {
	return product_;
}

std::string AliRequest::resourcePath() const {
	return resourcePath_;
}

void AliRequest::setResourcePath(const std::string & path) {
	resourcePath_ = path;
}

void AliRequest::setHttpMethod(HttpRequest::Method method) {
  httpMethod_ = method;
}

HttpRequest::Method AliRequest::httpMethod() const {
  return httpMethod_;
}

}
