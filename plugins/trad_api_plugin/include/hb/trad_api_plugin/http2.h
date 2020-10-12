#pragma once

#include <curl/curl.h>
#include <string>
#include <hb/log/log.h>
#include <hb/trad_api_plugin/trad_api_error.h>

using namespace std;
namespace hb{ namespace http2{
    static size_t writeFun(void *input, size_t uSize, size_t uCount, void *avg) {
        size_t uLen = uSize*uCount;
        std::string &sBuffer = *reinterpret_cast<std::string *> (avg);
        sBuffer.append(reinterpret_cast<const char *> (input), uLen);
        return uLen;
    }
    struct http_requet_type
    {
        string method;
        string url;
        string cert_file;
        int expired_seconds = {20};
        string body;
    };

    int http_call_sync(const http_requet_type& request, string &response) {
        CURLcode code = curl_global_init(CURL_GLOBAL_DEFAULT);
        if (code != CURLE_OK) {
            hb::plugin::trad_api_exception e;
            e.msg("curl_global_init Err: %d", code);
            hb_throw(e);
        }
        CURL* pCurl = curl_easy_init();
        if (pCurl == NULL) {
            hb::plugin::trad_api_exception e;
            e.msg("curl_easy_init Err");
            hb_throw(e);
        }
        std::string sBuffer;
        curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 1L);
        curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 1L);
        curl_easy_setopt(pCurl, CURLOPT_URL, request.url.c_str()); // 访问的URL
        if(request.cert_file != ""){
            curl_easy_setopt(pCurl, CURLOPT_SSLKEYTYPE, "PEM");
            curl_easy_setopt(pCurl, CURLOPT_CAINFO, request.cert_file.c_str());
        }
        if (request.method == "POST") {
            curl_easy_setopt(pCurl, CURLOPT_POST, 1);
            curl_slist *plist = curl_slist_append(NULL, "Content-Type:application/json;charset=UTF-8");
            curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, plist);
        } else {
            curl_slist *plist = curl_slist_append(NULL, "Content-Type:application/x-www-form-urlencoded");
            curl_easy_setopt(pCurl, CURLOPT_HTTPHEADER, plist);
        }
        curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, request.expired_seconds); // 超时(单位S)
        curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, &writeFun); // !数据回调函数
        curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &sBuffer); // !数据回调函数的参，一般为Buffer或文件fd
        if (request.method == "POST") {
            curl_easy_setopt(pCurl, CURLOPT_POSTFIELDSIZE, request.body.size());
            curl_easy_setopt(pCurl, CURLOPT_COPYPOSTFIELDS, request.body.c_str());
        }
        code = curl_easy_perform(pCurl);
        response = sBuffer;
        // if (code != CURLE_OK) {
        //     curl_easy_cleanup(pCurl);
        //     curl_global_cleanup();
        //     hb_throw("curl_easy_perform() Err: %d", code);
        // }
        curl_easy_cleanup(pCurl);
        curl_global_cleanup();
        return code;
    }
} }
