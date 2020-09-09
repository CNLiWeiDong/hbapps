//
// Created by 李卫东 on 2019-02-18.
//

#ifndef SIGN_THREAD_POOL_PLUGIN_IMPL_H
#define SIGN_THREAD_POOL_PLUGIN_IMPL_H

#include "boost/asio.hpp"
#include <memory>
#include <thread>
#include <functional>
#include <string>
#include <fc/logging/logging.h>

using namespace std;

namespace hb{ namespace plugin {
        class thread_pool_plugin_impl {
        public:
            thread_pool_plugin_impl(const int &thread_num);
            ~thread_pool_plugin_impl();
            shared_ptr<boost::asio::io_service> get_io_service() { return _io_service; }
            void post(const string &task_name,const function<void()> &task){
                _io_service->post([=]()mutable throw(){
                    try {
                        log_info<<"begin do thread pool work, task name:"<<task_name;
                        task();
                        log_info<<"end do thread pool work, task name:"<<task_name;
                    }catch(...){
                        LOG_ERROR("do thread pool work error:%s", task_name.c_str());
                        log_throw(task_name.c_str());
                    }
                });
            }
            template <typename T, typename  C>
            void post(const string &task_name,
                      const T &task,
                      const C &callback){
                _io_service->post([=]()mutable throw(){
                    try {
                        log_info<<"begin do thread pool work, task name:"<<task_name;
                        callback(task());
                        log_info<<"end do thread pool work, task name:"<<task_name;
                    }catch(...){
                        LOG_ERROR("do thread pool work error:%s",task_name.c_str());
                        log_throw(task_name.c_str());
                    }
                });
            }
            template <typename T, typename  C>
            void post(const string &task_name, const boost::asio::io_service &main_io_server,
                      const T &task,
                      const C &callback){
                _io_service->post([=,&main_io_server]()mutable throw(){
                    try {
                        log_info<<"begin do thread pool work, task name:"<<task_name;
                        // callback(task());
                        auto r = task();
                        // if (decltype(task()) == typeid(void)) {

                        // }
                        main_io_server.post([=]()mutable throw(){
                            callback(r);
                        });
                        log_info<<"end do thread pool work, task name:"<<task_name;
                    }catch(...){
                        LOG_ERROR("do thread pool work error:%s",task_name.c_str());
                        log_throw(task_name.c_str());
                    }
                });
            }
        private:
            shared_ptr<boost::asio::io_service> _io_service;
            std::vector< std::thread > _workers;
            shared_ptr<boost::asio::io_service::work> _service_work;
        };
} }

#endif //SIGN_THREAD_POOL_PLUGIN_IMPL_H
