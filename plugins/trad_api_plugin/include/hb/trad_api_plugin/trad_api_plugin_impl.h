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
#include <hb/trad_api_plugin/ApiSignature.h>

namespace hb{ namespace plugin {
    using namespace std;
    using namespace appbase;

    enum trad_order_status{
        order_processing = 0,
        order_portion_closed = 1,
        order_complet_closed = 2,
        order_untrad_closed = 3
    };
    struct order_result_type
    {
        trad_order_status status; 
        double price;
        double number;
        double amount;
        double fee;
        uint64_t create_time;
    };
    struct price_result_type{
        bool status = {false};
        double close_price;
        double max_buy_price;
        double max_buy_num;
        double max_sale_price;
        double max_sale_num;
    };
    struct trad_api_data_type {
        string host;
        string port;
        string access_key;
        string secret_key;
        string target_pair;
        int expired_seconds;
        int sleep_milliseconds;
        string cert_pem;
        string url_query_pirce;  
        string url_query_account; 
        string url_query_order; 
        string url_query_order_client; 
        string url_cancel_order;
        string url_new_order;
    };
    class trad_api_plugin_impl: public std::enable_shared_from_this<trad_api_plugin_impl>{
        trad_api_data_type data_;
        string account_id_;
    public:
        void data(const trad_api_data_type& val){ data_=val; }
        trad_api_data_type& data(){ return data_; }
        void account_id(const string& val){ account_id_=val; }
        string account_id(){ return account_id_; }
    public:
        price_result_type get_price();
        string get_account();
        const order_result_type get_order_status(const string& order_id);
        const string query_order_id_by_client(const string& client_id);
        void cancell_order(const string& order_id);
        string new_order(const double& num, const double& price, const bool& is_buy = true);
        string new_buy_order(const double& num, const double& price){
            return new_order(num,price);
        }
        string new_sale_order(const double& num, const double& price){
            return new_order(num,price,false);
        }
        void start();
    private:
        string generate_order_id();
    public:
        trad_api_plugin_impl() {

        }
        ~trad_api_plugin_impl();
    };
} }
