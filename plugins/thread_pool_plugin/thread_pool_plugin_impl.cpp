//
// Created by 李卫东 on 2019-02-18.
//

#include "hb/thread_pool_plugin/thread_pool_plugin_impl.h"

namespace hb{ namespace plugin {
        thread_pool_plugin_impl::thread_pool_plugin_impl(const int &thread_num){
            _io_service = make_shared<boost::asio::io_service>();
            _service_work = make_shared<boost::asio::io_service::work>(*_io_service);
            for(int i=0;i<thread_num;i++)
            {
                _workers.emplace_back([this](){
                    this->_io_service->run();
                });
            }
        }
        thread_pool_plugin_impl::~thread_pool_plugin_impl() {
            _io_service->stop();
            for (auto& w : _workers) {
                w.join();
            }
        }
} }