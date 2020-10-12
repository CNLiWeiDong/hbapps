//
// Created by 李卫东 on 2019-02-18.
//

#ifndef SIGN_HTTP_SERVER_PLUGIN_IMPL_H
#define SIGN_HTTP_SERVER_PLUGIN_IMPL_H

#include "boost/asio.hpp"
#include <memory>
#include <vector>
#include <map>
#include <thread>
#include <functional>
#include <string>
#include <hb/log/log.h>
#include <hb/http_server_plugin/listener.h>
#include <boost/signals2/signal.hpp>

namespace hb{ namespace plugin {
    using namespace std;
    using namespace hb::http_server;
    
    struct http_options {
        bool https_enable;
        string host;
        int port;
        uint64_t body_size;
        uint32_t expires_seonds;
        string certificate_file;
        string private_file;
        string private_password;
        // vector<string> enable_paths;
    };
    template <typename T> 
    struct all_element 
    { 
        typedef T result_type; 
        
        template <typename InputIterator> 
        T operator()(InputIterator first, InputIterator last) const 
        { 
            return T(first, last); 
        } 
    };

    typedef boost::signals2::signal<deal_result_type(const deal_param_type &), all_element<deal_results_type>> signal_type;
    class http_server_plugin_impl: public std::enable_shared_from_this<http_server_plugin_impl>{
    public:
        http_server_plugin_impl(const http_options &opt):
            http_options_(opt)
        {};
        ~http_server_plugin_impl();
        void start();
        void connect_request(const string &signal_name, std::function<deal_result_type(const deal_param_type &)> request_deal_fun );
    private:
        signal_type& regist_signal(const string &signal_name);
        deal_results_type notify_request_deal(const deal_param_type &req_param);
        void start_http_server(std::shared_ptr<net::io_context> ioc);
        void start_https_server(std::shared_ptr<net::io_context> ioc);
    private:
        http_options http_options_;
        map<string, signal_type> registed_signals_; 
        mutex signals_mutex_;
    };
} }

#endif //SIGN_HTTP_SERVER_PLUGIN_IMPL_H
