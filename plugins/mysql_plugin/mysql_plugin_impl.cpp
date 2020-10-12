//
// Created by 李卫东 on 2019-02-26.
//

#include <hb/mysql_plugin/mysql_plugin_impl.h>
#include <hb/log_plugin/log_plugin.h>
#include <hb/mysql_plugin/mysql_error.h>

namespace hb { namespace plugin {


        shared_ptr<my_connection> mysql_plugin_impl::connect(const char* db, const char* server,
                                                       const char* user, const char* password,
                                                       unsigned int port){
            string pool_key = server + to_string(port)  + db;
            shared_ptr<my_connection> con_ptr = nullptr;
            do{
                std::unique_lock<std::mutex> lock(pool_mutex_);
                auto it = all_pools_.find(pool_key);
                if(it==all_pools_.end()){
                    mysql_pool_type pool;
                    all_pools_[pool_key] = pool;
                }
                auto &pools = all_pools_[pool_key];
                if(!pools.empty()){
                    for(auto it = pools.begin();it!=pools.end();){
                        con_ptr = pools.front();
                        it = pools.erase(it);
                        if(con_ptr->status())
                            break;
                        else
                            con_ptr = nullptr;
                    }
                }
            }while(0);
            if(con_ptr && con_ptr->status()){
                con_ptr->thread_start();
                con_ptr->open();
                return con_ptr;
            }
            con_ptr = make_shared<my_connection>();
            //con_ptr->thread_start() 创建时会自动启动
            if(!con_ptr->connect(db,server,user,password,port)) {
                hb::plugin::mysql_exception e;
                e.msg("connect sql server error!");
                hb_throw(e);
            }
            return con_ptr;
        }
        void mysql_plugin_impl::close(const shared_ptr<my_connection> &con){
            if(con->is_stoped())
                return;
            string pool_key = con->server()+to_string(con->port()) + con->db();
            do{
                std::unique_lock<std::mutex> lock(pool_mutex_);
                auto it = all_pools_.find(pool_key);
                if(it==all_pools_.end()){
                    mysql_pool_type pool;
                    all_pools_[pool_key] = pool;
                }
                auto &pools = all_pools_[pool_key];
                if(pools.size()<pool_max_size_){
                    con->stop();
                    pools.push_back(con);
                    con->thread_end();
                }

            }while(0);
        }
} }