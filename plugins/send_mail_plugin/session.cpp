#include <hb/send_mail_plugin/session.h>
#include <hb/log/log.h>
#include <hb/crypto/base.hpp>
#include <hb/send_mail_plugin/send_mail_error.h>

namespace hb{ namespace plugin {
    void session::mail(){
        log_info <<"session::mail begin!!!";
        if(host_.size()==0){
            hb::plugin::send_mail_exception e;
            e.msg("Error! Need set host!");
            hb_throw(e);
        }
        if(to_.size()==0){
            hb::plugin::send_mail_exception e;
            e.msg("Error! Need set mail to mail!");
            hb_throw(e);
        }
        boost::asio::io_service ioc;
        tcp::resolver resolver(ioc);
        tcp::resolver::query query(host_,port_);
        tcp::resolver::iterator it_endpt=resolver.resolve(query);
        tcp::resolver::iterator end;
        if(it_endpt==end){
            hb::plugin::send_mail_exception e;
            e.msg("Error! resolve host error!");
            hb_throw(e);
        }
        // tcp::socket sock(ioc);
        boost::asio::ssl::context ctx(ssl::context::tlsv12_client);
        ctx.set_verify_mode(ssl::verify_none);
        boost::asio::ssl::stream<boost::asio::ip::tcp::socket> sock(ioc, ctx); 
        boost::system::error_code error;
        // sock.connect(*it_endpt++,error);
        sock.lowest_layer().connect(*it_endpt++,error);
        sock.lowest_layer().set_option(boost::asio::ip::tcp::no_delay(true));
        if(error){
            // sock.close();
            sock.shutdown(error);
            hb::plugin::send_mail_exception e;
            e.msg("HOST can't be connected!");
            hb_throw(e);
        }
        sock.handshake(boost::asio::ssl::stream_base::client);
        send(sock,"HELO "+user_+"\r\n");
        log_info << "mail response:" << get_response(sock);
        
        send(sock,"auth login\r\n");
        log_info << "mail response:" << get_response(sock);
        send(sock,hb::crypto::base64(user_)+"\r\n");
        log_info << "mail response:" << get_response(sock);
        send(sock, hb::crypto::base64(pass_)+"\r\n");
        log_info << "mail response:" << get_response(sock);
        
        send(sock,"mail from: <"+user_+">\r\n");
        log_info << "mail response:" << get_response(sock);
        send(sock,"rcpt to: <"+to_+">\r\n");
        log_info << "mail response:" << get_response(sock);
        send(sock,"data\r\n");
        log_info << "mail response:" << get_response(sock);
        send(sock,"From:"+user_+"\r\n");
        //log_info << "mail response:" << get_response(sock);
        send(sock,"subject:"+subject_+"\r\nMime-Version: 1.0\r\n"
                    "Content-Type: multipart/mixed;   "
                    "boundary=\"INVT\"\r\n\r\n");
        //log_info << "mail response:" << get_response(sock);
        send(sock,"--INVT\r\nContent-Type: text/plain;\r\n  "
                    "charset=\"utf8\"\r\n\r\n"+content_+"\r\n\r\n");
        //log_info << "mail response:" << get_response(sock);
        send(sock,"\r\n.\r\n");
        log_info << "mail response:" << get_response(sock);
        send(sock,"quit\r\n");
        log_info << "mail response:" << get_response(sock);
        sock.shutdown(error);
        log_info <<"session::mail end!!!";
    }
} };