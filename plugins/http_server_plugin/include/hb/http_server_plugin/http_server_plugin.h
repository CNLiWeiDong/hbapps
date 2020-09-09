//
// Created by 李卫东 on 2019-02-19.
//

#ifndef SIGN_HTTP_SEVER_PLUGIN_H
#define SIGN_HTTP_SEVER_PLUGIN_H


#include <functional>
#include <string>
#include "appbase/application.hpp"
#include "hb/logging_plugin/logging_plugin.h"
#include <hb/http_server_plugin/http_server_plugin_impl.h>
#include <hb/thread_pool_plugin/thread_pool_plugin.h>

using namespace std;

namespace hb{ namespace plugin {
    using namespace appbase;
    class http_server_plugin_api{
    public:
        http_server_plugin_api(shared_ptr<http_server_plugin_impl> _impl):impl(_impl){}
        void connect(const string &signal_name, std::function<deal_result_type(const deal_param_type &)> request_deal_fun ){
            impl->connect_request(signal_name,request_deal_fun);
        }
    private:
        shared_ptr<http_server_plugin_impl> impl;
    };
    class http_server_plugin : public appbase::plugin<http_server_plugin> {
        public:
            APPBASE_PLUGIN_REQUIRES((logging_plugin)(thread_pool_plugin))
            http_server_plugin();
            virtual ~http_server_plugin();
            virtual void set_program_options(options_description&, options_description&) override;
            void plugin_initialize(const variables_map&);
            void plugin_startup();
            void plugin_shutdown();
            shared_ptr<http_server_plugin_api> get_api(){
                return make_shared<http_server_plugin_api>(my);
            }
        private:
            shared_ptr<http_server_plugin_impl> my;
    };
}}

#endif //SIGN_HTTP_SEVER_PLUGIN_H
