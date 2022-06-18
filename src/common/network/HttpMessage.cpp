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

#include <iostream>
#include <string.h>
#include <utility>
//#include <iostream> // for debug
#include <algorithm>
#include "HttpMessage.h"

#if defined(_WIN32) && defined(_MSC_VER)
#define strcasecmp _stricmp
#define strncasecmp _strnicmp
#else
#include <strings.h>
#endif

namespace AlibabaNlsCommon {

std::string KnownHeaderMapper[] = {
  "Accept",
  "Accept-Charset",
  "Accept-Encoding",
  "Accept-Language",
  "Authorization",
  "Connection",
  "Content-Length",
  "Content-MD5",
  "Content-Type",
  "Date",
  "Host",
  "Server",
  "User-Agent"
};

HttpMessage::HttpMessage(const HttpMessage &other) :
  body_(other.body_),
  headers_(other.headers_) 
{
}

HttpMessage::HttpMessage(HttpMessage &&rhs) noexcept :
  body_(std::move(rhs.body_)),
  headers_(std::move(rhs.headers_)) 
{
  //std::cout << "HttpMessage move construct" << std::endl;
  // 处理rhs
  rhs.body_.clear();
  rhs.headers_.clear();
}

HttpMessage& HttpMessage::operator=(const HttpMessage &other) {
  if (this != &other) {
    body_ = other.body_;
    headers_ = other.headers_;
  }
  return *this;
}

HttpMessage& HttpMessage::operator=(HttpMessage &&rhs) noexcept
{
  //std::cout << "HttpMessage move assign" << std::endl;
  if (this != &rhs) {
    body_ = std::move(rhs.body_);
    headers_ = std::move(rhs.headers_);

    // 处理rhs
    rhs.body_.clear();
    rhs.headers_.clear();
  }
  return *this;
}

void HttpMessage::addHeader(const HeaderNameType & name,
                            const HeaderValueType & value) {
  setHeader(name, value);
}

void HttpMessage::addHeader(KnownHeader header,
                            const HeaderValueType & value) {
  setHeader(header, value);
}

HttpMessage::HeaderValueType HttpMessage::header(
    const HeaderNameType & name) const {
  HeaderCollectionIterator it = headers_.find(name);
  if (it != headers_.end()) {
    return it->second;
  } else {
    return std::string();
  }
}

HttpMessage::HeaderCollection HttpMessage::headers() const {
  return headers_;
}

void HttpMessage::removeHeader(const HeaderNameType & name) {
  headers_.erase(name);
}

void HttpMessage::removeHeader(KnownHeader header) {
	removeHeader(KnownHeaderMapper[header]);
}

void HttpMessage::setHeader(const HeaderNameType & name,
                            const HeaderValueType & value) {
  headers_[name] = value;
}

void HttpMessage::setHeader(KnownHeader header, const std::string & value) {
  setHeader(KnownHeaderMapper[header], value);
}

HttpMessage::~HttpMessage() 
{

}

std::string& HttpMessage::getBody() {
  return body_;
}

const std::string& HttpMessage::getBody() const {
  return body_;
}

size_t HttpMessage::getBodySize() const {
  return body_.size();
}

bool HttpMessage::hasBody() const {
	return (body_.size() != 0);
}

HttpMessage::HeaderValueType HttpMessage::header(KnownHeader header) const {
  return this->header(KnownHeaderMapper[header]);
}

void HttpMessage::setBody(const char *data, size_t size) {
  body_.append(data, size);
}  

bool HttpMessage::nocaseLess::operator()(
    const std::string & s1, const std::string & s2) const {
  return strcasecmp(s1.c_str(), s2.c_str()) < 0;
}

}
