//
// Created by 李卫东 on 2019-02-18.
//
#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <string>
#include <atomic>
#include <fc/logging/logging.h>
#include <appbase/application.hpp>

namespace hb{ namespace plugin {
    using namespace std;
    using namespace appbase;
    class send_mail_plugin_impl: public std::enable_shared_from_this<send_mail_plugin_impl>{
        string host_;
        string port_;
        string user_;
        string pass_;
    public:
        void host(const string& host){ host_=host; }
        const std::string& host(){ return host_; }
        void port(const string& port){ port_=port; }
        const std::string& port(){ return port_; }
        void user(const string& user){ user_=user; }
        const std::string& user(){ return user_; }
        void pass(const string& pass){ pass_=pass; }
        const std::string& pass(){ return pass_; }
    public:
        send_mail_plugin_impl() {

        }
        ~send_mail_plugin_impl();
        void send_mail(const string &to, const string &subject, const string &content);
    };
} }
