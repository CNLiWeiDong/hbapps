#pragma once
#include <hb/http_server_plugin/common.h>

namespace hb { namespace http_server {
    // Accepts incoming connections and launches the sessions
    class listener : public std::enable_shared_from_this<listener>
    {
        std::shared_ptr<net::io_context> ioc_;
        std::shared_ptr<ssl::context> ctx_;
        tcp::acceptor acceptor_;
        tcp::endpoint endpoint_;
        bool have_ssl_;
        uint64_t body_limit_ = {1048576};
        uint32_t expires_seconds_ = {30};
        deal_fun_type deal_fun_ = { nullptr };
    public:
        listener(
            std::shared_ptr<net::io_context> ioc,
            std::shared_ptr<ssl::context> ctx,
            tcp::endpoint endpoint)
            : ioc_(ioc)
            , ctx_(ctx)
            , acceptor_(net::make_strand(*ioc))
            , endpoint_(endpoint)
            , have_ssl_(true) 
        {
            init_acceptor();
        };
        listener(
            std::shared_ptr<net::io_context> ioc,
            tcp::endpoint endpoint)
            : ioc_(ioc)
            , ctx_(nullptr)
            , acceptor_(net::make_strand(*ioc))
            , endpoint_(endpoint) 
            , have_ssl_(false)
        {
            init_acceptor();
        };
        // Start accepting incoming connections
        void run();
        void set_deal_fun(const deal_fun_type& fun) { deal_fun_ = fun; };
        void set_body_limit(const uint64_t& limit) { body_limit_ = limit; };
        void set_expires_seconds(const uint32_t& secondes) { expires_seconds_ = secondes; };
    private:
        void init_acceptor();
        void do_accept();
        void on_accept(beast::error_code ec, tcp::socket socket);
    };
}}