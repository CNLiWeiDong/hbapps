#include <hb/http_server_plugin/listener.h>
#include <hb/http_server_plugin/session_ssl.h>
#include <hb/http_server_plugin/session.h>

namespace hb { namespace http_server { 
        void listener::init_acceptor()
        {
            beast::error_code ec;
            // Open the acceptor
            acceptor_.open(endpoint_.protocol(), ec);
            if(ec)
            {
                LOG_FATAL("open acceptor error: %s",ec.message().c_str());
            }
            // Allow address reuse
            acceptor_.set_option(net::socket_base::reuse_address(true), ec);
            if(ec)
            {
                LOG_FATAL("set acceptor option error: %s",ec.message().c_str());
            }
            // Bind to the server address
            acceptor_.bind(endpoint_, ec);
            if(ec)
            {
                LOG_FATAL("bind acceptor error: %s",ec.message().c_str());
            }
            // Start listening for connections
            acceptor_.listen(
                net::socket_base::max_listen_connections, ec);
            if(ec)
            {
                LOG_FATAL("listen error: %s",ec.message().c_str());
            }
        }
        // Start accepting incoming connections
        void listener::run()
        {
            do_accept();
        }
        void listener::do_accept()
        {
            // The new connection gets its own strand
            acceptor_.async_accept(
                net::make_strand(*ioc_),
                beast::bind_front_handler(
                    &listener::on_accept,
                    shared_from_this()));
        }
        void listener::on_accept(beast::error_code ec, tcp::socket socket)
        {
            if(ec)
            {
                LOG_ERROR("accept error: %s",ec.message().c_str());
            }
            else
            {
                if (have_ssl_) {
                    auto sess = std::make_shared<session_ssl>(std::move(socket), *ctx_);
                    sess->set_deal_fun(deal_fun_);
                    sess->run();
                }
                else {
                    auto sess = std::make_shared<session>(std::move(socket));
                    sess->set_deal_fun(deal_fun_);
                    sess->set_body_limit(body_limit_);
                    sess->set_expires_seconds(expires_seconds_);
                    sess->run();
                }
            }
            // Accept another connection
            do_accept();
        }
}}