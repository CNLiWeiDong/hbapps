//
// Created by 李卫东 on 2019-02-18.
//
#pragma once

#include <memory>
#include <vector>
#include <map>
#include <thread>
#include <functional>
#include <string>
#include <fc/logging/logging.h>
#include <appbase/application.hpp>
#include <hb/grid_db_plugin/grid_db_plugin.h>
#include <hb/send_mail_plugin/send_mail_plugin.h>
#include <hb/trad_api_plugin/trad_api_plugin.h>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <hb/grid_trad_plugin/delvier_order.h>
#include <hb/grid_trad_plugin/new_order.h>

namespace hb{ namespace plugin {
    using namespace std;
    using namespace appbase;

    class grid_trad_plugin_impl: public std::enable_shared_from_this<grid_trad_plugin_impl>{
    public:
        grid_trad_plugin_impl() {
            delvier_order_ = make_shared<delvier_order>();
            new_order_ = make_shared<new_order>();
        }
        ~grid_trad_plugin_impl();
        void start();
    public:
        void intervals_seconds(const int& val) { intervals_seconds_ = val; }
        void order_cancel_seconds(const int& val) { delvier_order_->order_cancel_seconds(val); }
        void fee_is_free(const bool& val) { delvier_order_->fee_is_free(val); }
        void min_number_num(const double& val) { new_order_->min_number_num(val);}
        void every_max_buy_counts(const int& val) { new_order_->every_max_buy_counts(val); }
        void every_max_sale_counts(const int& val) { new_order_->every_max_sale_counts(val); }
        void sale_sleep_seconds(const int& val) { new_order_->sale_sleep_seconds(val); }
        void buy_sleep_seconds(const int& val) { new_order_->buy_sleep_seconds(val); }
        void improve_deliver_premium(const double& val) { new_order_->improve_deliver_premium(val);}
        void add_tactics(const tactic_config_type &item) { new_order_->add_tactics(item); }
    private:
        void deal_loop();
        void loop();
    private:
        int intervals_seconds_ = {20};
        shared_ptr<boost::asio::deadline_timer> deadline_updater_;
        shared_ptr<delvier_order> delvier_order_;
        shared_ptr<new_order> new_order_;
    };
} }

