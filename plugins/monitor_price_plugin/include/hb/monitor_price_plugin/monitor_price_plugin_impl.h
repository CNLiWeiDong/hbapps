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
#include <hb/log/log.h>
#include <appbase/application.hpp>
#include <hb/monitor_price_plugin/monitor_pairs.h>
#include <hb/monitor_price_plugin/monitor_singles.h>
#include <hb/monitor_price_plugin/monitor_targets.h>

namespace hb{ namespace plugin {
    using namespace std;
    using namespace appbase;
    
    
    class monitor_price_plugin_impl: public std::enable_shared_from_this<monitor_price_plugin_impl>{
        shared_ptr<monitor_targets> targets_; 
        shared_ptr<monitor_singles> singles_; 
        shared_ptr<monitor_pairs> pairs_; 
        uint32_t intervals_seconds_ = {20};
        uint32_t delay_update_seconds_ = {5};
        uint32_t max_request_error_time_ = {5};
        uint64_t sendmsg_seconds_ = {2*60*60};
        shared_ptr<boost::asio::deadline_timer> deadline_query_;
        shared_ptr<boost::asio::deadline_timer> deadline_updater_;
    public:
        void add_target(const string &id, const target_type &target) { targets_->add_target(id, target);}
        void add_single(const singles_type & single) { singles_->add_single(single);}
        void add_pairs(const pairs_type & p) {pairs_->add_pairs(p);}
        void sendmsg_seconds(int t) {
            sendmsg_seconds_ = t;
            singles_->sendmsg_seconds(t); 
            pairs_->sendmsg_seconds(t);
        }
        void senderror_seconds(const int &t) {targets_->senderror_seconds(t);}
        void intervals_seconds(int t) {intervals_seconds_ = t;}
        void delay_update_seconds(int t) {delay_update_seconds_ = t; targets_->delay_update_seconds(t);}
        void max_request_error_time(int t) {max_request_error_time_ = t; targets_->max_request_error_time(t);}
        void request_error_mail_to(const string &v) {targets_->request_error_mail_to(v);}
    public:
        monitor_price_plugin_impl() {
            targets_ = make_shared<monitor_targets>();
            singles_ = make_shared<monitor_singles>();
            pairs_ = make_shared<monitor_pairs>();
        }
        ~monitor_price_plugin_impl();
        void start();
        void deal_monitor();
        void loop();
    };
} }
