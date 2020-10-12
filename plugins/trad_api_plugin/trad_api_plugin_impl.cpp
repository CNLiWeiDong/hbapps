//
// Created by 李卫东 on 2019-02-19.
//
#include <hb/trad_api_plugin/trad_api_plugin_impl.h>
#include <hb/https/https.h>
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/format.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/thread/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <hb/trad_api_plugin/http2.h>
#include <hb/trad_api_plugin/trad_api_error.h>

namespace hb{ namespace plugin {
        using namespace boost::property_tree;

        trad_api_plugin_impl::~trad_api_plugin_impl(){
            
        }
        string trad_api_plugin_impl::generate_order_id() {
                boost::uuids::random_generator uuid_v4;
                boost::uuids::uuid id = uuid_v4();
                return boost::uuids::to_string(id);
        }
        void trad_api_plugin_impl::start() {
                auto id = get_account();
                account_id(id);
        }
        void sleep_millisec(const int &millisec) {
                boost::thread::sleep(boost::get_system_time() + boost::posix_time::milliseconds(millisec));
        }
        string trad_api_plugin_impl::get_account() {
                sleep_millisec(data_.sleep_milliseconds);
                string url = "https://"+data_.host+data_.url_query_account+"?";
                url += Huobi::ApiSignature::buildSignaturePath(data_.host, data_.access_key, data_.secret_key,data_.url_query_account,"GET","");
                hb::http2::http_requet_type req;
                req.method = "GET";
                req.url = url;
                req.expired_seconds = data_.expired_seconds;
                string res_body;
                auto code = hb::http2::http_call_sync(std::move(req), res_body);
                if(code){
                        hb::plugin::trad_api_exception e;
                        e.msg("get_account http error:%d", code);
                        hb_throw(e);
                }
                log_info<<"get_account:" <<code << res_body;
                ptree res_pt;
                stringstream res_stream(res_body);
                read_json(res_stream,res_pt);
                auto status = res_pt.get_optional<string>("status");
                if(!status || *status!="ok"){
                        log_error<<"get_account error:"<< res_body;
                        hb::plugin::trad_api_exception e;
                        e.msg("get_account status error!");
                        hb_throw(e);
                }
                ptree account_arr = res_pt.get_child("data");
                for(auto it = account_arr.begin(); it!=account_arr.end(); it++) {
                        if(it->second.get<string>("type")=="spot" && 
                                        it->second.get<string>("state")=="working"){
                                return it->second.get<string>("id");
                        }
                }
                hb::plugin::trad_api_exception e;
                e.msg("get_account is empty!");
                hb_throw(e);
                return "";
        }
        const order_result_type trad_api_plugin_impl::get_order_status(const string& order_id) {
                sleep_millisec(data_.sleep_milliseconds);
                string url_query_order = boost::replace_all_copy(data_.url_query_order,"{order-id}",order_id);
                string url = "https://"+data_.host+url_query_order+"?";
                url += Huobi::ApiSignature::buildSignaturePath(data_.host, data_.access_key, data_.secret_key,url_query_order,"GET","");
                hb::http2::http_requet_type req;
                req.method = "GET";
                req.url = url;
                req.expired_seconds = data_.expired_seconds;
                log_info<<url;
                string res_body;
                auto code = hb::http2::http_call_sync(std::move(req), res_body);
                if(code){
                        hb::plugin::trad_api_exception e;
                        e.msg("get_order_status http error:%d", code);
                        hb_throw(e);
                }
                log_info<<"get_order_status:" <<code << res_body;
                ptree res_pt;
                stringstream res_stream(res_body);
                read_json(res_stream,res_pt);
                auto status = res_pt.get_optional<string>("status");
                if(!status || *status!="ok"){
                        log_error<<"get_order_status error:"<< res_body;
                        hb::plugin::trad_api_exception e;
                        e.msg("get_order_status status error!");
                        hb_throw(e);
                }
                ptree d = res_pt.get_child("data");
                const string state = d.get<string>("state");
                trad_order_status order_status;
                if(state=="filled")
                        order_status = trad_order_status::order_complet_closed;
                else if(state=="partial-canceled")
                        order_status = trad_order_status::order_portion_closed;
                else if(state=="canceled")
                        order_status = trad_order_status::order_untrad_closed;
                else
                      order_status= trad_order_status::order_processing; 
                
                double order_fee      = atof(d.get<string>("field-fees").c_str());
                double order_amount   = atof(d.get<string>("field-cash-amount").c_str());
                double order_number   = atof(d.get<string>("field-amount").c_str());
                double order_price    = 0;
                if(order_number>0){
                        order_price    = order_amount/order_number;
                }
                // log_info<<order_status<<"|"<<order_price<<"|"<<order_number<<"|"<<order_amount<<"|"<<order_fee<<"|";
                order_result_type result;
                result.status = order_status;
                result.price = order_price;
                result.number = order_number;
                result.amount = order_amount;
                result.fee = order_fee;
                return result;
        }
        void trad_api_plugin_impl::cancell_order(const string& order_id){
                sleep_millisec(data_.sleep_milliseconds);
                string url_cancel_order = boost::replace_all_copy(data_.url_cancel_order,"{order-id}",order_id);
                string url = "https://"+data_.host+url_cancel_order+"?";
                url += Huobi::ApiSignature::buildSignaturePath(data_.host, data_.access_key, data_.secret_key,url_cancel_order,"POST","");
                hb::http2::http_requet_type req;
                req.method = "POST";
                req.url = url;
                req.expired_seconds = data_.expired_seconds;
                log_info<<url;
                string res_body;
                auto code = hb::http2::http_call_sync(std::move(req), res_body);
                if(code){
                        hb::plugin::trad_api_exception e;
                        e.msg("cancell_order http error:%d", code);
                        hb_throw(e);
                }
                log_info<<"cancell_order:" <<code << res_body;
                ptree res_pt;
                stringstream res_stream(res_body);
                read_json(res_stream,res_pt);
                auto status = res_pt.get_optional<string>("status");
                if(!status || *status!="ok"){
                        log_error<<"cancell_order error:"<< res_body;
                        hb::plugin::trad_api_exception e;
                        e.msg("cancell_order status error!");
                        hb_throw(e);
                }
        }
        const string trad_api_plugin_impl::query_order_id_by_client(const string& client_id) {
                sleep_millisec(data_.sleep_milliseconds);
                string url = "https://"+data_.host+data_.url_query_order_client+"?";
                string url_param = "clientOrderId="+client_id;
                url += Huobi::ApiSignature::buildSignaturePath(data_.host, data_.access_key, data_.secret_key,data_.url_query_order_client,"GET",url_param.c_str());
                url += "&"+url_param;
                hb::http2::http_requet_type req;
                req.method = "GET";
                req.url = url;
                //如果是第一次返回失败，第二次大概率返回失败，所以第二次确认时间要长一点
                req.expired_seconds = data_.expired_seconds+30;
                log_info<<url;
                string res_body;
                auto code = hb::http2::http_call_sync(std::move(req), res_body);
                if(code){
                        log_error<<"cancell_order error:"<< res_body;
                        hb::plugin::trad_api_exception e;
                        e.msg("query_order_id_by_client http error:%d", code);
                        hb_throw(e);
                }
                log_info<<"query_order_id_by_client body:" <<code << res_body;
                ptree res_pt;
                stringstream res_stream(res_body);
                read_json(res_stream,res_pt);
                auto status = res_pt.get_optional<string>("status");
                if(!status || *status!="ok"){
                        log_error<<"query_order_id_by_client error:"<< res_body;
                        log_error<<"cancell_order error:"<< res_body;
                        hb::plugin::trad_api_exception e;
                        e.msg("query_order_id_by_client status error!");
                        hb_throw(e);
                }
                ptree d = res_pt.get_child("data");
                return d.get<string>("id");
        }
        string trad_api_plugin_impl::new_order(const double& num, const double& price, const bool& is_buy) {
                sleep_millisec(data_.sleep_milliseconds);
                string client_order_id = generate_order_id();
                log_info<<"new_order client_order_id:"<<client_order_id;
                string url = "https://"+data_.host+data_.url_new_order+"?";
                url += Huobi::ApiSignature::buildSignaturePath(data_.host, data_.access_key, data_.secret_key,data_.url_new_order,"POST","");
                hb::http2::http_requet_type req;
                req.method = "POST";
                req.url = url;
                req.expired_seconds = data_.expired_seconds;
                //req body
                ptree pt_req;
                pt_req.put("account-id",account_id_);
                pt_req.put("amount",(boost::format("%.4f")%num).str());
                pt_req.put("price",(boost::format("%.4f")%price).str());
                pt_req.put("source","spot-api");
                pt_req.put("symbol",data_.target_pair);
                if(is_buy)
                        pt_req.put("type","buy-limit");
                else
                        pt_req.put("type","sell-limit");
                pt_req.put("client-order-id",client_order_id);
                stringstream req_stream;
                write_json(req_stream,pt_req);
                req.body = req_stream.str();
                log_info<<"new order info:" << req.body;
                //res body
                string res_body;
                auto code = hb::http2::http_call_sync(std::move(req), res_body);
                log_info<<"new_order:" <<code << res_body;
                if(code){
                        //可能提交成功，但返回失败需要查询一次
                        return query_order_id_by_client(client_order_id);
                }
                ptree res_pt;
                stringstream res_stream(res_body);
                read_json(res_stream,res_pt);
                auto status = res_pt.get_optional<string>("status");
                if(!status || *status!="ok"){
                        log_error<<"new_order error:"<< res_body;
                        hb::plugin::trad_api_exception e;
                        e.msg("new_order status error!");
                        hb_throw(e);
                }
                return res_pt.get<string>("data");
        }
        
        price_result_type trad_api_plugin_impl::get_price(){
                sleep_millisec(data_.sleep_milliseconds);
                hb::http2::http_requet_type req;
                req.method = "GET";
                req.url = "https://"+data_.host+data_.url_query_pirce+data_.target_pair;
                req.expired_seconds = data_.expired_seconds;
                string res_body;
                auto code = hb::http2::http_call_sync(std::move(req), res_body);
                if(code){
                        hb::plugin::trad_api_exception e;
                        e.msg("get_price http error:%d", code);
                        hb_throw(e);
                }
                log_info<<"get_price:"<<code<<"="<<res_body;
                ptree res_pt;
                stringstream res_stream(res_body);
                read_json(res_stream,res_pt);
                auto status = res_pt.get_optional<string>("status");
                if(!status || *status!="ok"){
                        log_error<<"get_price error:"<< res_body;
                        hb::plugin::trad_api_exception e;
                        e.msg("get_price status error!");
                        hb_throw(e);
                }
                boost::format fmt("market.%1%.detail.merged");
                fmt%data_.target_pair;
                if(res_pt.get<string>("ch")!=fmt.str()){
                        hb::plugin::trad_api_exception e;
                        e.msg("get_price price is not market.eosusdt.detail.merged!");
                        hb_throw(e);
                }
                auto tick = res_pt.get_child("tick");
                double close_price = tick.get<double>("close");
                ptree max_sale_arr = tick.get_child("ask");
                ptree max_buy_arr = tick.get_child("bid");
                auto sale_iterator = max_sale_arr.begin();
                auto buy_iterator = max_buy_arr.begin();
                
                price_result_type result = {
                        status:         true,
                        close_price:    close_price,
                        max_buy_price:  buy_iterator++->second.get_value<double>(),
                        max_buy_num:    buy_iterator->second.get_value<double>(),
                        max_sale_price: sale_iterator++->second.get_value<double>(),
                        max_sale_num:   sale_iterator->second.get_value<double>()
                };
                return std::move(result);
        }
       
} }