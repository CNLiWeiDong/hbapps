#pragma once

#include <string>
#include <cctype>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <boost/asio.hpp>  
#include <boost/asio/ssl.hpp> 

namespace hb{ namespace plugin {
    using namespace std;
    using namespace boost::asio;
    using boost::asio::ip::tcp;

    class session{
        public:
            ~session(){};
            void host(const string& host){ host_=host; }
            void port(const string& port){ port_=port; }
            void user(const string& user){ user_=user; }
            void pass(const string& pass){ pass_=pass; }
            void to(const string& to){ to_=to; }
            void subject(const string& subject){ subject_=subject; }
            void content(const string& content){ content_=content; }
            void send(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& sock, const string& msg){ boost::asio::write(sock, boost::asio::buffer(msg)); }
            const string get_response(boost::asio::ssl::stream<boost::asio::ip::tcp::socket>& sock){
                boost::asio::streambuf buffer;
                string msg;
                boost::asio::read_until(sock, buffer, "\r\n");
                std::istream in(&buffer);
                std::getline(in, msg);
                return msg;
            }
            void mail();

        private:
            string host_="";
            string port_="25";
            string user_="";
            string pass_="";
            string to_="";
            string subject_="";
            string content_="";
    };
    
} };