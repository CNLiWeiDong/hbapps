//
// Created by 李卫东 on 2019-02-18.
//
#pragma once

#include <memory>
#include <vector>
#include <map>
#include <set>
#include <thread>
#include <functional>
#include <string>
#include <hb/log/log.h>
#include <hb/grid_db_plugin/grid_db_plugin.h>
#include <hb/send_mail_plugin/send_mail_plugin.h>
#include <hb/trad_api_plugin/trad_api_plugin.h>
#include <hb/grid_trad_plugin/grid_trad_error.h>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "appbase/application.hpp"

namespace hb{ namespace plugin {
    using namespace std;
    using namespace appbase;

    struct tactic_config_type {
        string seg_id;
        double sale_price;
        double buy_price;
        double sale_number;
    };
    class new_order: public std::enable_shared_from_this<new_order>{
    public:
        void add_tactics(const tactic_config_type &item) {
            auto iter = tactics_.find(item.seg_id);
            if(iter != tactics_.end())
            {
                grid_trad_exception e;
                e.msg("%s tactic already existed!", item.seg_id);
                hb_throw(e);
            }
            tactics_.insert(decltype(tactics_)::value_type(item.seg_id,std::move(item)));
        }
        void improve_deliver_premium(const double& val) { improve_deliver_premium_ = val; }
        void min_number_num(const double& val) { min_number_num_ = val; }
        void every_max_buy_counts(const int& val) { every_max_buy_counts_ = val; }
        void every_max_sale_counts(const int& val) { every_max_sale_counts_ = val; }
        void sale_sleep_seconds(const int& val) { sale_sleep_seconds_ = val; }
        void buy_sleep_seconds(const int& val) { buy_sleep_seconds_ = val; }
        bool deal(std::set<std::string> dealing_segs);
    private:
        price_result_type query_price(const bool& is_buy=true);
        void add_sale_order(const double& price,const double& sale_num, const tactic_config_type &tactic);
        void add_buy_order(const double& price,const double& buy_amount, const tactic_config_type &tactic);
        const double get_sold_number(const string& seg_id,const sale_tx_array_type& sold_list);
        const double get_free_amount(const string& seg_id,const sale_tx_array_type& sold_list);
    private:
        double improve_deliver_premium_ = {0.1/100};
        double min_number_num_ = {1.0};
        int every_max_buy_counts_ = {1};
        int every_max_sale_counts_ = {4};
        int sale_sleep_seconds_ = {0};
        int buy_sleep_seconds_ = {5*60};
        uint64_t last_buy_time_ = {0};
        uint64_t last_sale_time_ = {0};
        map<string,tactic_config_type> tactics_;
    };
} }

