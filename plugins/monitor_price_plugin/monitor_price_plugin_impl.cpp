//
// Created by 李卫东 on 2019-02-19.
//
#include <hb/monitor_price_plugin/monitor_price_plugin_impl.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <math.h>
#include <fc/http/http.h>
#include <fc/https/https.h>
#include <hb/send_mail_plugin/send_mail_plugin.h>
#include <sstream>

namespace hb{ namespace plugin {
        
        monitor_price_plugin_impl::~monitor_price_plugin_impl(){

        }
        
        void monitor_price_plugin_impl::deal_monitor() {
            try{
                log_info<<"begin deal_monitor";
                singles_->deal(targets_);
                pairs_->deal(targets_);
                log_info<<"end deal_monitor";
            }catch(...){
                log_throw("deal_monitor");
            }
        }
        
        void monitor_price_plugin_impl::loop() {
            auto self = shared_from_this();
            auto & io = app().get_io_service();
            deadline_query_ = make_shared<boost::asio::deadline_timer>(io, boost::posix_time::seconds(intervals_seconds_));
            // deadline_query_->expires_from_now(boost::posix_time::seconds(2));
	        deadline_query_->async_wait([self, &io](const boost::system::error_code &ec){
                self->targets_->deal_query();
                self->deadline_updater_ = make_shared<boost::asio::deadline_timer>(io, boost::posix_time::seconds(self->delay_update_seconds_)+boost::posix_time::millisec(100));
                    self->deadline_updater_->async_wait([self](const boost::system::error_code &ec){
                        self->deal_monitor();
                    });
                self->loop();
            });
        }
        void monitor_price_plugin_impl::start(){
            loop();
        }
} }