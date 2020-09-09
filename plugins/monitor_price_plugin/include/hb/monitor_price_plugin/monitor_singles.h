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
#include <hb/monitor_price_plugin/monitor_targets.h>

namespace hb{ namespace plugin {
    using namespace std;
    using namespace appbase;
    struct singles_type
    {
        string id ;
        double min;
        double max;
        string phone;
        string mail;
        uint64_t send_msg_time;
        bool active;
    };
    
    class monitor_singles: public std::enable_shared_from_this<monitor_singles>{
        vector<singles_type> singles_list_;
        uint64_t sendmsg_seconds_ = {2*60*60};
    public:
        void add_single(const singles_type & single) {
            singles_list_.push_back(single);
        }
        void sendmsg_seconds(int t) {sendmsg_seconds_ = t;}
    public:
        void deal(const shared_ptr<monitor_targets> &targets);
    };
} }
