//
// Created by 李卫东 on 2019-02-26.
//

#pragma once

#include <memory>
#include <appbase/application.hpp>
#include <hb/grid_db_plugin/grid_db_types.h>
#include <hb/mysql_plugin/mysql_plugin.h>

namespace hb{ namespace plugin {
        using namespace std;
        using namespace appbase;

        class grid_db_plugin_impl: public std::enable_shared_from_this<grid_db_plugin_impl>{
        public:
            grid_db_plugin_impl(){}
            void set_db_info(const string &host, const string &name, const string &user, const string &pass, const int &port) {
                db_host_ = host;
                db_name_ = name;
                db_user_ = user;
                db_passwd_ = pass;
                db_port_ = port;
            }
        public:
            sale_tx_array_type get_delivered_sale_array();
            sale_tx_array_type get_new_sale_array();
            buy_tx_array_type get_new_buy_array();
            void deliver_sale_tx(const sale_tx_type &tx);
            void deliver_buy_tx(const buy_tx_type &tx);
            void update_buy_status(mysqlpp::Query &query,const buy_tx_type &tx);
            void add_new_sale_tx(const sale_tx_type &tx);
            void add_new_buy_tx(const buy_tx_type &tx);
        private:
            sale_tx_array_type query_sale_tx(const string &sql_str);
            std::shared_ptr<hb::mysql_plugin::my_connection> get_connection();
            void close_connection(const std::shared_ptr<hb::mysql_plugin::my_connection> &con);
        private:
            //防止复制
            grid_db_plugin_impl(const grid_db_plugin_impl&& c){}
            grid_db_plugin_impl(const grid_db_plugin_impl &c){}
            void operator =(const grid_db_plugin_impl &c){}
            void operator =(const grid_db_plugin_impl &&c){}
        private:
            string db_host_;
            string db_name_;
            string db_user_;
            string db_passwd_;
            int db_port_;
        };

}}