#pragma once
#include <hb/http_server_plugin/common.h>

namespace hb { namespace http_server {
    // Handles an HTTP server connection
    class session_ssl : public std::enable_shared_from_this<session_ssl>
    {
        // This is the C++11 equivalent of a generic lambda.
        // The function object is used to send an HTTP message.
        struct send_lambda
        {
            session_ssl& self_;
            explicit send_lambda(session_ssl& self)
                : self_(self)
            {
            }
            template<bool isRequest, class Body, class Fields>
            void operator()(http::message<isRequest, Body, Fields>&& msg) const
            {
                auto sp = std::make_shared<
                    http::message<isRequest, Body, Fields>>(std::move(msg));
                self_.res_ = sp;
                http::async_write(
                    self_.stream_,
                    *sp,
                    beast::bind_front_handler(
                        &session_ssl::on_write,
                        self_.shared_from_this(),
                        sp->need_eof()));
            }
        };
        beast::ssl_stream<beast::tcp_stream> stream_;
        beast::flat_buffer buffer_;
        http::request_parser<http::string_body> req_parser_;
        std::shared_ptr<void> res_;
        send_lambda lambda_;
        uint64_t body_limit_ = {1048576};
        uint32_t expires_seconds_ = {30};
        deal_fun_type deal_fun_ = { nullptr };
    public:
        friend struct send_lambda;
        // Take ownership of the socket
        explicit session_ssl(tcp::socket&& socket, ssl::context& ctx) 
            : stream_(std::move(socket), ctx)
            , lambda_(*this)
        {
            req_parser_.body_limit(body_limit_);
        }
        // Start the asynchronous operation
        void run();
        void on_run();
        void on_handshake(beast::error_code ec);
        void do_read();
        void on_read(beast::error_code ec, std::size_t bytes_transferred);
        void on_write(bool close, beast::error_code ec, std::size_t bytes_transferred);
        void do_close();
        void do_close_read();
        void do_close_write();
        void set_deal_fun(const deal_fun_type& fun) { deal_fun_ = fun; };
        void set_body_limit(const uint64_t& limit) { 
            body_limit_ = limit; 
            req_parser_.body_limit(limit); 
        };
        void set_expires_seconds(const uint32_t& secondes) { expires_seconds_ = secondes; };
    };
}}