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
#include <string>
#include "HttpRequest.h"

namespace AlibabaNlsCommon {

HttpRequest::HttpRequest(const Url &url, Method method) : HttpMessage(),
                                                          url_(url),
                                                          method_(method) {
}

HttpRequest::~HttpRequest() {}

HttpRequest::Method HttpRequest::method() const {
  return method_;
}

void HttpRequest::setMethod(Method method) {
  method_ = method;
}

void HttpRequest::setUrl(const Url &url) {
  url_ = url;
}

Url HttpRequest::url() const {
  return url_;
}


std::string HttpMethodToString(HttpRequest::Method method) {
  switch (method) {
    case HttpRequest::Head:
      return "HEAD";
      break;
    case HttpRequest::Post:
      return "POST";
      break;
    case HttpRequest::Put:
      return "PUT";
      break;
    case HttpRequest::Delete:
      return "DELETE";
      break;
    case HttpRequest::Connect:
      return "CONNECT";
      break;
    case HttpRequest::Options:
      return "OPTIONS";
      break;
    case HttpRequest::Patch:
      return "PATCH";
      break;
    case HttpRequest::Trace:
      return "TRACE";
      break;
    case HttpRequest::Get:
    default:
      return "GET";
      break;
  }
}

}
