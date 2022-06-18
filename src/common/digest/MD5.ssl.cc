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

#include "MD5.cxx.h"
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/md5.h>
#ifdef OPENSSL_IS_BORINGSSL 
#include <openssl/base64.h>
#endif


namespace SQ
{

std::string ComputeContentMD5(const std::string& data) 
{
    return ComputeContentMD5(data.c_str(), data.size());
}

std::string ComputeContentMD5(const char * data, size_t size)
{
    if (!data) {
        return "";
    }

    unsigned char md[MD5_DIGEST_LENGTH];
    MD5(reinterpret_cast<const unsigned char*>(data), size, (unsigned char*)&md);
     
    char encodedData[100];
    EVP_EncodeBlock(reinterpret_cast<unsigned char*>(encodedData), md, MD5_DIGEST_LENGTH);
    return encodedData;
}

std::string ComputeContentMD5(std::istream& stream) 
{
    auto ctx = EVP_MD_CTX_create();

    unsigned char md_value[EVP_MAX_MD_SIZE];
    unsigned int md_len = 0;

    EVP_MD_CTX_init(ctx);
#ifndef OPENSSL_IS_BORINGSSL 
    EVP_MD_CTX_set_flags(ctx, EVP_MD_CTX_FLAG_NON_FIPS_ALLOW);
#endif
    EVP_DigestInit_ex(ctx, EVP_md5(), nullptr);

    auto currentPos = stream.tellg();
    if (currentPos == static_cast<std::streampos>(-1)) {
        currentPos = 0;
        stream.clear();
    }
    stream.seekg(0, stream.beg);

    char streamBuffer[2048];
    while (stream.good())
    {
        stream.read(streamBuffer, 2048);
        auto bytesRead = stream.gcount();

        if (bytesRead > 0)
        {
            EVP_DigestUpdate(ctx, streamBuffer, static_cast<size_t>(bytesRead));
        }
    }

    EVP_DigestFinal_ex(ctx, md_value, &md_len);
    EVP_MD_CTX_destroy(ctx);
    stream.clear();
    stream.seekg(currentPos, stream.beg);

    //Based64
    char encodedData[100];
    EVP_EncodeBlock(reinterpret_cast<unsigned char*>(encodedData), md_value, md_len);
    return encodedData;
}
} // end namespace digest