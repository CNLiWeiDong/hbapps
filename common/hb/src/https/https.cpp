#include <hb/https/https.h>
#include <hb/error/exception.h>

namespace hb {  namespace https {
    void https::load_certificates()
    {
        if(cert_str_.empty() && ctx_==nullptr){
            ctx_ = make_shared<ssl::context>(ssl::context::tlsv12_client);
            ctx_->set_verify_mode(ssl::verify_none);
            return;
        }
        if(ctx_)
            return;

        boost::system::error_code ec;
        ctx_ = make_shared<ssl::context>(ssl::context::tlsv12_client);
        ctx_->add_certificate_authority(
            boost::asio::buffer(cert_str_.data(), cert_str_.size()), ec);
        // Verify the remote server's certificate
        ctx_->set_verify_mode(ssl::verify_peer);
        if(ec){

        }
            throw boost::system::system_error{ec};
    }
    int https::request()
    {
        hb_try
            // The io_context is required for all I/O
            net::io_context ioc;
            boost::system::error_code ec;
            load_certificates();
            // These objects perform our I/O
            tcp::resolver resolver(ioc);
            beast::ssl_stream<beast::tcp_stream> stream(ioc, *ctx_);
            beast::get_lowest_layer(stream).expires_after(std::chrono::seconds(expires_));
            // Set SNI Hostname (many hosts need this to handshake successfully)
            if(! SSL_set_tlsext_host_name(stream.native_handle(), host_.c_str()))
            {
                beast::error_code ec{static_cast<int>(::ERR_get_error()), net::error::get_ssl_category()};
                throw beast::system_error{ec};
            }
            // Look up the domain name
            auto const results = resolver.resolve(host_, port_);
            // Make the connection on the IP address we get from a lookup
            beast::get_lowest_layer(stream).connect(results);
            // Perform the SSL handshake
            stream.handshake(ssl::stream_base::client);
            // Send the HTTP request to the remote host
            beast::http::write(stream, req_);
            // This buffer is used for reading and must be persisted
            beast::flat_buffer buffer;
            // Declare a container to hold the response
            beast::http::response<beast::http::string_body> res;
            // Receive the HTTP response
            beast::http::read(stream, buffer, res);
            // Write the message to standard out
            res_body_ = res.body();
            // Gracefully close the stream
            stream.shutdown(ec);
            // if(ec == net::error::eof)
            // {
            //     ec = {};
            // }
            // if(ec)
            //     throw beast::system_error{ec};
            return 200;
        hb_catch([&](const auto &e) {
            res_body_ = log_throw("do https request error", e);
            return 500;
        })
    }
    int https::get()
    {
        req_.method(beast::http::verb::get);
        return request();
    }
    int https::post()
    {
        req_.method(beast::http::verb::post); 
        req_.set(beast::http::field::content_length, boost::lexical_cast<std::string>(req_body_.size()));
        req_.set(beast::http::field::body, req_body_);
        return request();
    }
    void https::post(net::io_context& ioc, const response_fun_type &callback) {
        load_certificates();
        req_.method(beast::http::verb::post);
        req_.set(beast::http::field::content_length, boost::lexical_cast<std::string>(req_body_.size()));
        req_.set(beast::http::field::body, req_body_);
        req_.prepare_payload();
        auto sess = std::make_shared<session>(ioc, *ctx_, req_, callback);
        sess->request(host_, port_); 
    }
    void https::get(net::io_context& ioc, const response_fun_type &callback) {
        load_certificates();
        req_.method(beast::http::verb::get);
        auto sess = std::make_shared<session>(ioc, *ctx_, req_, callback);
        sess->request(host_, port_); 
    }
} }