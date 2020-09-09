//
// Created by 李卫东 on 2019-02-26.
//

#pragma once

#include <memory>
#include <appbase/application.hpp>
#include <hb/mysql_plugin/my_connection.h>

namespace hb{ namespace plugin {
        using namespace std;
        using namespace appbase;
        using namespace hb::mysql_plugin;

        typedef deque<shared_ptr<my_connection>> mysql_pool_type;
        class mysql_plugin_impl {
        public:
            mysql_plugin_impl(){}
            void pool_max_size(const int & size) { pool_max_size_ = size; }
            shared_ptr<my_connection> connect(const char* db = 0, const char* server = 0,
                                                const char* user = 0, const char* password = 0,
                                                unsigned int port = 0);
            void close(const shared_ptr<my_connection> &con);
            // std::shared_ptr<mysqlpp::Transaction> transaction(const char* db = 0, const char* server = 0,
            //                                     const char* user = 0, const char* password = 0,
            //                                     unsigned int port = 0){
            //         auto con = connect(db,server, user,password,port);
            //         return make_shared<mysqlpp::Transaction>(*con);
            // }
        private:
            //防止复制
            mysql_plugin_impl(const mysql_plugin_impl&& c){}
            mysql_plugin_impl(const mysql_plugin_impl &c){}
            void operator =(const mysql_plugin_impl &c){}
            void operator =(const mysql_plugin_impl &&c){}
        private:
            map<string,mysql_pool_type> all_pools_;
            mutex pool_mutex_;
            int pool_max_size_;
        };

}}