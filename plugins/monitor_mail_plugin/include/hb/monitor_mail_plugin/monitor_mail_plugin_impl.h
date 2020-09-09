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
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace hb{ namespace plugin {
    using namespace std;
    using namespace appbase;

    class monitor_mail_plugin_impl: public std::enable_shared_from_this<monitor_mail_plugin_impl>{
    
    public:
        void start();
    private:
        void send_test_mail();
        void loop();
    public:
        monitor_mail_plugin_impl() {

        }
        ~monitor_mail_plugin_impl();
        void notify_mail_to(const string& val) { notify_mail_to_ = val; }
        void notify_time(const int& val) { notify_time_ = val; }
    private:
        shared_ptr<boost::asio::deadline_timer> deadline_updater_;
        string notify_mail_to_;
        int notify_time_ = {0};
        int sended_msg_day_ = {0};
    };
} }
