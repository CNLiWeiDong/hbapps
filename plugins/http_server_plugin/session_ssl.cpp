#include <hb/http_server_plugin/session_ssl.h>

namespace hb { namespace http_server { 
    // Start the asynchronous operation
    void session_ssl::run()
    {
        // We need to be executing within a strand to perform async operations
        // on the I/O objects in this session. Although not strictly necessary
        // for single-threaded contexts, this example code is written to be
        // thread-safe by default.
        net::dispatch(
            stream_.get_executor(),
            beast::bind_front_handler(
                &session_ssl::on_run,
                shared_from_this()));
    }
    void session_ssl::on_run()
    {
        // Set the timeout.
        beast::get_lowest_layer(stream_).expires_after(
            std::chrono::seconds(expires_seconds_));
        // Perform the SSL handshake
        stream_.async_handshake(
            ssl::stream_base::server,
            beast::bind_front_handler(
                &session_ssl::on_handshake,
                shared_from_this()));
    }
    void session_ssl::on_handshake(beast::error_code ec)
    {
        if(ec){
            LOG_ERROR("handshake error: %s",ec.message().c_str());
            return;
        }
        do_read();
    }
    void session_ssl::do_read()
    {
        // Make the request empty before reading,
        // otherwise the operation behavior is undefined.
        // req_ = {};
        // Set the timeout.
        beast::get_lowest_layer(stream_).expires_after(std::chrono::seconds(expires_seconds_));
        // Read a request
        http::async_read(stream_, buffer_, req_parser_,
            beast::bind_front_handler(
                &session_ssl::on_read,
                shared_from_this()));
    }
    void session_ssl::on_read(beast::error_code ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);
        // This means they closed the connection
        if(ec == http::error::end_of_stream)
            return do_close_read();
        if(ec){
            LOG_ERROR("read error: %s",ec.message().c_str());
            return do_close();
        }
        // Send the response
        handle_request(std::move(req_parser_.get()), deal_fun_, lambda_);
    }
    void session_ssl::on_write(bool close, beast::error_code ec, std::size_t bytes_transferred)
    {
        boost::ignore_unused(bytes_transferred);
        if(ec){
            LOG_ERROR("write error: %s",ec.message().c_str());
            return do_close();
        }
        if(close)
        {
            return do_close_write();
        }
        // We're done with the response so delete it
        res_ = nullptr;
        // Read another request
        // do_read();
    }

    void session_ssl::do_close()
    {
        beast::error_code ec;
        stream_.shutdown(ec);
    }
    void session_ssl::do_close_read()
    {
        beast::error_code ec;
        stream_.next_layer().socket().shutdown(tcp::socket::shutdown_receive, ec);
    }
    void session_ssl::do_close_write()
    {
        beast::error_code ec;
        stream_.next_layer().socket().shutdown(tcp::socket::shutdown_send, ec);
    }
}}