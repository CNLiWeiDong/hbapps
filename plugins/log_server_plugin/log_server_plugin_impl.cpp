//
// Created by 李卫东 on 2019-02-19.
//
#include <hb/log_server_plugin/log_server_plugin_impl.h>

namespace hb{ namespace plugin {
        log_server_plugin_impl::log_server_plugin_impl(const int& port)
        {
            socket_ = make_shared<udp::socket>(app().get_io_service(), udp::endpoint(udp::v4(), port));
        }
        log_server_plugin_impl::~log_server_plugin_impl(){
            
        }
        void log_server_plugin_impl::do_receive()
        {
                socket_->async_receive_from( boost::asio::buffer(read_data_, max_length), sender_endpoint_,
                        [this](boost::system::error_code ec, std::size_t bytes_recvd)
                        {
                                if (!ec && bytes_recvd > 0)
                                {
                                        // read_data_.resize(bytes_recvd);
                                        const string log_str(read_data_, bytes_recvd);
                                        auto pos = log_str.find(msg_after_);
                                        if(pos!=string::npos){
                                                log_info<<log_str.substr(pos+msg_after_.size());
                                        } else
                                        {
                                                log_info<<log_str;
                                        }
                                }
                                do_receive();
                        }
                );
        }
        void log_server_plugin_impl::start() {
                do_receive();
        }
       
} }