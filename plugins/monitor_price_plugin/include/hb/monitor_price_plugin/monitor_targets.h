//
// Created by 李卫东 on 2019-02-18.
//
#pragma once

#include <vector>
#include <map>
#include <thread>
#include <memory>
#include <functional>
#include <string>
#include <atomic>
#include <fc/logging/logging.h>
#include <appbase/application.hpp>
#include <fc/time/time.h>
#include <hb/send_mail_plugin/send_mail_plugin.h>
#include <sstream>

namespace hb{ namespace plugin {
    using namespace std;
    using namespace appbase;

    enum target_status {
        target_status_begin,
        target_status_ok,
        target_status_request_error,
        target_status_regex_value_error,
        target_status_regex_date_error,
        target_status_regex_content_error,
        target_status_throw_error,
        target_status_week_error,
        target_status_day_minutes_error
    };
    struct target_type
    {
        string id;
        string server_type;
        string host;
        string port;
        string target;
        int min_week_day;
        int max_week_day;
        int min_day_minutes;
        int max_day_minutes;
        string format_value;
        string format_date;
        string format_contents;
        double value;
        string date;
        vector<string> contents;
        target_status status;
        uint64_t update_time;
        bool active;
        int request_error_times = 0;
        uint64_t last_send_error_time = 0;
    };
   
    class monitor_targets: public std::enable_shared_from_this<monitor_targets>{
        map<string, target_type> all_targets_list_;
        uint32_t delay_update_seconds_ = {5};
        uint32_t max_request_error_time_ = {5};
        uint64_t senderror_seconds_ = {2*60*60};
        string request_error_mail_to_;
    public:
        void delay_update_seconds(const int &t) {delay_update_seconds_ = t;}
        void max_request_error_time(const int &t) {max_request_error_time_ = t;}
        void senderror_seconds(int t) {senderror_seconds_ = t;}
        void request_error_mail_to(const string &v) {request_error_mail_to_ = v;}
        map<string, target_type> & all_tagets() {return all_targets_list_;}
        void add_target(const string &id, const target_type &target) {
            auto iter = all_targets_list_.find(id);
            if(iter != all_targets_list_.end())
            {
                LOG_FATAL("%s target already existed!", id.c_str());
            }
            all_targets_list_.insert(decltype(all_targets_list_)::value_type(id,std::move(target)));
        }
    public:
        void deal_query();
        void set_target(target_type &item, const int &status, const std::string &res);
        void add_request_error_times(target_type &item);
    };
} }
