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
#include <hb/log/log.h>
#include <hb/grid_db_plugin/grid_db_plugin.h>
#include <hb/send_mail_plugin/send_mail_plugin.h>
#include <hb/trad_api_plugin/trad_api_plugin.h>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include "appbase/application.hpp"

namespace hb{ namespace plugin {
    using namespace std;
    using namespace appbase;

    
    class delvier_order: public std::enable_shared_from_this<delvier_order>{
    
    public:
        std::set<std::string> deal();
        void order_cancel_seconds(const int& val) { order_cancel_seconds_ = val; }
        void fee_is_free(const bool& val) { fee_is_free_ = val; }
    private:
        void deal_selling_order(const sale_tx_array_type& txs);
        void complete_sale_order(const order_result_type& order_result, const sale_tx_type& tx);
        void deal_buying_order(const buy_tx_array_type& txs);
        void complete_buy_order(const order_result_type& order_result, const buy_tx_type& tx);
    private:
        int order_cancel_seconds_ = {5*60};
        bool fee_is_free_ = {true};
    };
} }

