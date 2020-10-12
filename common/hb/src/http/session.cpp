#include <hb/http/session.h>

namespace hb {  namespace http {
    void session::request(const string &host, const string &port)
    {
        // Look up the domain name
        resolver_.async_resolve(
            host,
            port,
            beast::bind_front_handler(
                &session::on_resolve,
                shared_from_this()));
    }

    void session::on_resolve( beast::error_code ec, tcp::resolver::results_type results)
    {
        if(ec)
            return fail(ec, "resolve");
        // Set a timeout on the operation
        stream_.expires_after(std::chrono::seconds(expires_));
        // Make the connection on the IP address we get from a lookup
        stream_.async_connect(
            results,
            beast::bind_front_handler(
                &session::on_connect,
                shared_from_this()));
    }

    void session::on_connect(beast::error_code ec, tcp::resolver::results_type::endpoint_type)
    {
        if(ec)
            return fail(ec, "connect");

        // Set a timeout on the operation
        stream_.expires_after(std::chrono::seconds(expires_));

        // Send the HTTP request to the remote host
        beast::http::async_write(stream_, req_,
            beast::bind_front_handler(
                &session::on_write,
                shared_from_this()));
    }

    void session::on_write( beast::error_code ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);
        if(ec)
            return fail(ec, "write");
        // Receive the HTTP response
        beast::http::async_read(stream_, buffer_, res_,
            beast::bind_front_handler(
                &session::on_read,
                shared_from_this()));
    }
    void session::on_read( beast::error_code ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);
        if(ec)
            return fail(ec, "read");
        // Write the message to standard out
        // std::cout << res_ << std::endl;
        response_fun_(200, res_.body());
        stream_.expires_after(std::chrono::seconds(expires_));
        // Gracefully close the socket
        stream_.socket().shutdown(tcp::socket::shutdown_both, ec);
    }
} }