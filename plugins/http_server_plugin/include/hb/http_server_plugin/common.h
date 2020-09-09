#pragma once
#include <cstddef>
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <boost/asio.hpp>
#include <boost/asio/buffer.hpp>
#include <boost/asio/ssl/context.hpp>
#include <boost/asio/dispatch.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/config.hpp>
#include <hb/logging_plugin/logging_plugin.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>


namespace hb { namespace http_server {
    namespace beast = boost::beast;         // from <boost/beast.hpp>
    namespace http = beast::http;           // from <boost/beast/http.hpp>
    namespace net = boost::asio;            // from <boost/asio.hpp>
    namespace ssl = boost::asio::ssl;       // from <boost/asio/ssl.hpp>
    using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
    using namespace boost::property_tree;

    typedef std::vector<ptree> deal_results_type;
    typedef ptree deal_result_type;
    typedef ptree deal_param_type;
    typedef std::function<deal_results_type(const deal_param_type&)> deal_fun_type;

    //  Load a signed certificate into the ssl context, and configure
    void load_server_certificate(boost::asio::ssl::context& ctx);
    // This function produces an HTTP response for the given
    // request. The type of the response object depends on the
    // contents of the request, so the interface requires the
    // caller to pass a generic lambda for receiving the response.
    // response
    // {
    //     "jsonrpc": "2.0",
    //     "id": 0,
    //     "results": [{
    //             "type":"error",
    //             "state":0,
    //             "msg":"",
    //             "method":""
    //             "data":""
    //     }]
    // }
    //request
    // {
    //     "id": 0,
    //     "jsonrpc": "2.0",
    //     "method": "flight_getBlockByNumber",
    //     "params": 
    // }
    template<class Body, class Allocator, class Send>
    void handle_request(http::request<Body, http::basic_fields<Allocator>>&& req, const deal_fun_type& deal,
        Send&& send)
    {
        ptree pt_res;
        pt_res.put("id",0);
        pt_res.put("jsonrpc","2.0");
        ptree results;
        auto const add_error_result = [&](const string& type, const int &state,const string &msg){
            ptree item;
            item.put("type",type);
            item.put("state",state);
            item.put("msg",msg);
            item.put("data","");
            results.push_back(make_pair("",item));
        };
        auto const send_response = [&](){
            pt_res.put_child("results",results);
            stringstream stream;
            write_json(stream,pt_res);
            http::response<http::string_body> res{http::status::bad_request, req.version()};
            res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
            res.set(http::field::content_type, "application/text");
            res.keep_alive(req.keep_alive());
            res.body() = std::string(stream.str());
            res.prepare_payload();
            send(std::move(res));
            // {
            //     boost::regex exp("\"(null|true|false|[0-9]+(\\.[0-9]+)?)\"");
            //     std::stringstream ss;
            //     boost::property_tree::json_parser::write_json(ss, *pt);
            //     std::string rv = boost::regex_replace(ss.str(), exp, "$1");

            //     return rv;
            // }
        };
        try {
            ptree pt_req;
            log_info<<req.body();
            stringstream stream(req.body());
            read_json(stream,pt_req);
            auto id = pt_req.get_optional<int>("id");
            auto version = pt_req.get_optional<string>("jsonrpc");
            auto method = pt_req.get_optional<string>("method");
            if(!(id && version && method)){
                add_error_result("request-error", -1, "params need id:version:method");
                send_response();
                return;
            }
            auto del_results = deal(pt_req);
            if(del_results.size()==0){
                add_error_result("method-error", -1, "not support the method");
                send_response();
                return;
            }
            for(auto &item : del_results){
                item.put("type","success");
                item.put("state",0);
                item.put("msg","ok");
                item.put("method",method);
                results.push_back(std::make_pair("",item));
            }
            send_response();
        }catch(const std::exception &e){
            // cout<<boost::diagnostic_information(e)<<endl;
            add_error_result("server-error", -1, log_throw("do handle_request work error"));
            send_response();
        }catch(...){
            add_error_result("server-error", -1, log_throw("do handle_request work error..."));
            send_response();
        }
    }

}}