//
// Created by 李卫东 on 2019-02-19.
//
#include <hb/monitor_price_plugin/monitor_targets.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <math.h>
#include <hb/http/http.h>
#include <hb/https/https.h>
#include <hb/send_mail_plugin/send_mail_plugin.h>
#include <sstream>

namespace hb{ namespace plugin {
        bool regex_value(boost::regex &reg,boost::smatch &what, const std::string &res) {
            if ( boost::regex_search(res.begin(), res.end(), what, reg) )
            {
                if(what.size()<2){
                    LOG_ERROR("regex_search what.size error %lu", what.size());
                    return false;
                }
                log_info<<what[0];
                if(!what[1].matched){
                    LOG_ERROR("regex_search what[1].matched error %s", ((string)what[1]).c_str());
                    return false;
                }
            }else {
                LOG_ERROR("regex_search error");
                return false;
            }
            return true;
        }
        void monitor_targets::add_request_error_times(target_type &item) {
            item.request_error_times++;
            if(item.request_error_times>=max_request_error_time_) {
                item.request_error_times = 0;
                auto cur_seconds = hb::time::timestamp();
                if(cur_seconds - item.last_send_error_time >=senderror_seconds_){
                    item.last_send_error_time = cur_seconds;
                    log_info<<"【target request error】 " << item.id << item.request_error_times;
                    auto &mail_plugin = app().get_plugin<send_mail_plugin>();
                    auto api = mail_plugin.get_api();
                    std::ostringstream subject;
                    subject<<"[target request error]"<<item.id;
                    api->send_mail(request_error_mail_to_, subject.str(), subject.str()+"\n\rPlease pay attention to the target !!!");
                }
            }
        }
        void monitor_targets::set_target(target_type &item, const int &status, const std::string &res) {
            hb_try
                LOG_INFO("request callback %s, %s, %s", item.id.c_str(), item.server_type.c_str(), item.host.c_str());
                if(status!=200){
                    LOG_ERROR("[%s, %s, %s] request callback %d %s", 
                        item.id.c_str(), item.server_type.c_str(), item.host.c_str(),
                        status, res.c_str()
                    );
                    item.status = target_status::target_status_request_error;
                    add_request_error_times(item);
                    return;
                }
                boost::regex reg(item.format_value); 
                boost::smatch what;
                if(regex_value(reg, what, res)) {
                    item.status = target_status::target_status_ok;
                    item.update_time = hb::time::timestamp();
                    const string val = what[1];
                    item.value = std::atof(val.c_str());
                    LOG_INFO("set target value info %s, %f, %llu", item.id.c_str(), item.value, item.update_time);
                }else {
                    item.status = target_status::target_status_regex_value_error;
                    add_request_error_times(item);
                    return;
                }
                if(item.format_date!=""){
                    boost::regex reg(item.format_date); 
                    boost::smatch what;
                    if(regex_value(reg, what, res)) {
                        item.status = target_status::target_status_ok;
                        item.update_time = hb::time::timestamp();
                        item.date = what[1];
                        LOG_INFO("set target date info %s, %s, %llu", item.id.c_str(), item.date.c_str(), item.update_time);
                    }else {
                        item.status = target_status::target_status_regex_date_error;
                        add_request_error_times(item);
                        return;
                    }
                }
                if(item.format_contents!=""){
                    boost::regex reg(item.format_contents); 
                    boost::smatch what;
                    if(regex_value(reg, what, res)) {
                        item.status = target_status::target_status_ok;
                        item.update_time = hb::time::timestamp();
                        vector<string> contents;
                        for(int i=1;i<what.size();i++){
                            contents.push_back(what[i]);
                        }
                        item.contents = contents;
                        LOG_INFO("set target contents info %s, %u", item.id.c_str(), item.contents.size());
                    }else {
                        item.status = target_status::target_status_regex_content_error;
                        add_request_error_times(item);
                        return;
                    }
                }
                item.request_error_times = 0;
            hb_catch([&](const auto &e){
                item.status = target_status::target_status_throw_error;
                add_request_error_times(item);
                log_throw("monitor_targets::set_target", e);
            })
        }
        void monitor_targets::deal_query() {
            hb_try
                auto self = shared_from_this();
                auto & ioc = app().get_io_service();
                log_info<<"begin deal_query";
                const int week_day = hb::time::day_of_week();
                const int day_minutes = hb::time::hours_of_day()*60+hb::time::minutes_of_day();
                for(auto &it : all_targets_list_) {
                    auto &item = it.second;
                    if(!item.active)
                        continue;
                    if(!(item.min_week_day<=week_day && item.max_week_day>=week_day))
                    {
                        log_info<<item.id<< " is not in right week.";
                        item.status = target_status::target_status_week_error;
                        continue;
                    }
                    if(!(item.min_day_minutes<=day_minutes && item.max_day_minutes>=day_minutes))
                    {
                        log_info<<item.id<< " is not in right day minutes.";
                        item.status = target_status::target_status_day_minutes_error;
                        continue;
                    }
                    auto query_callback = [self,&item](const int &status, const std::string &res) {
                        self->set_target(item, status, res);
                    };
                    if(item.server_type=="https") 
                    {
                        hb::https::https hs(item.host, item.port, item.target, delay_update_seconds_);
                        hs.get(ioc, query_callback);
                    }else
                    {
                        hb::http::http h(item.host, item.port, item.target, delay_update_seconds_);
                        h.get(ioc, query_callback);
                    }
                }
                log_info<<"end deal_query";
            hb_catch([](const auto &e){
                log_throw("monitor_targets::deal_query", e);
            })
        }
} }