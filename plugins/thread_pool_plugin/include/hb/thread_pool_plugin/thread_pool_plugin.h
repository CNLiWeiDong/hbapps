//
// Created by 李卫东 on 2019-02-18.
//

#ifndef SIGN_THREAD_POOL_PLUGIN_H
#define SIGN_THREAD_POOL_PLUGIN_H

#include <memory>
#include <thread>
#include <functional>
#include <string>
#include <hb/log_plugin/log_plugin.h>
#include <appbase/application.hpp>
#include <hb/thread_pool_plugin/thread_pool_plugin_impl.h>

using namespace std;

namespace hb{ namespace plugin {
    using namespace appbase;
    class thread_pool_api{
    public:
        thread_pool_api(shared_ptr<thread_pool_plugin_impl> _impl):impl(_impl){}
        shared_ptr<boost::asio::io_context> get_io_context() { return impl->get_io_service(); }
        void post(const string &task_name,const function<void()> &task){
            impl->post(task_name,task);
        }
        template <typename T, typename  C>
        void post(const string &task_name,
                  const T &task,
                  const C &callback){
            impl->post<T,C>(task_name,task,callback);
        }
    private:
        shared_ptr<thread_pool_plugin_impl> impl;
    };
    class thread_pool_plugin : public appbase::plugin<thread_pool_plugin> {
        public:
            APPBASE_PLUGIN_REQUIRES((log_plugin))
            thread_pool_plugin();
            virtual ~thread_pool_plugin();
            virtual void set_program_options(options_description&, options_description&) override;
            void plugin_initialize(const variables_map&);
            void plugin_startup();
            void plugin_shutdown();
            shared_ptr<thread_pool_api> get_api(){
                return make_shared<thread_pool_api>(my);
            }
        private:
            shared_ptr<thread_pool_plugin_impl> my;
    };
}}

#endif //SIGN_THREAD_POOL_PLUGIN_H
