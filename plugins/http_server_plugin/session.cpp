#include <hb/http_server_plugin/session.h>

namespace hb { namespace http_server { 
    // Start the asynchronous operation
    void session::run()
    {
        net::dispatch(stream_.get_executor(),
                      beast::bind_front_handler(
                          &session::do_read,
                          shared_from_this()));
    }
    void session::do_read()
    {
        stream_.expires_after(std::chrono::seconds(expires_seconds_));
        // Read a request
        http::async_read(stream_, buffer_, req_parser_,
            beast::bind_front_handler(
                &session::on_read,
                shared_from_this()));
    }

    void session::on_read(beast::error_code ec, std::size_t bytes_transferred)
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

    void session::on_write(bool close, beast::error_code ec, std::size_t bytes_transferred)
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
    void session::do_close()
    {
        stream_.socket().close();
    }
    void session::do_close_read()
    {
        beast::error_code ec;
        stream_.socket().shutdown(tcp::socket::shutdown_receive, ec);
    }
    void session::do_close_write()
    {
        beast::error_code ec;
        stream_.socket().shutdown(tcp::socket::shutdown_send, ec);
    }
}}