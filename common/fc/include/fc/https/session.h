#pragma once
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <fc/logging/logging.h>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

namespace fc { namespace https {
    namespace beast = boost::beast;         // from <boost/beast.hpp>
    // namespace http = beast::http;           // from <boost/beast/http.hpp>
    namespace net = boost::asio;            // from <boost/asio.hpp>
    namespace ssl = net::ssl;               // from <boost/asio/ssl.hpp>
    using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
    using namespace std;

    typedef std::function<void(const int &status, const std::string& response)> response_fun_type;

    // Performs an HTTP GET and prints the response
    class session : public std::enable_shared_from_this<session>
    {
        tcp::resolver resolver_;
        beast::ssl_stream<beast::tcp_stream> stream_;
        beast::flat_buffer buffer_; // (Must persist between reads)
        beast::http::request<beast::http::string_body> req_;
        beast::http::response<beast::http::string_body> res_;
        response_fun_type response_fun_;
        int expires_ = {30};
    public:
        const int& expires() const { return expires_; }
        void expires(const int &expires) { expires_ = expires; }
    public:
        // Objects are constructed with a strand to
        // ensure that handlers do not execute concurrently.
        session(net::io_context& ioc, ssl::context& ctx, const beast::http::request<beast::http::string_body> req,  const response_fun_type &fun)
            : resolver_(net::make_strand(ioc))
            , stream_(net::make_strand(ioc), ctx)
            , req_(req)
            , response_fun_(fun)
        {
        }
        void request(const string &host, const string &port);
    private:
        void fail(beast::error_code ec, char const* what)
        {
            log_error<< what << ": " << ec.message();
            string message(what);
            message += ec.message();
            response_fun_(500, message);
        }
        void on_resolve(beast::error_code ec, tcp::resolver::results_type results);
        void on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type);
        void on_handshake(beast::error_code ec);
        void on_write(beast::error_code ec, std::size_t bytes_transferred);
        void on_read(beast::error_code ec, std::size_t bytes_transferred);
    };
} }