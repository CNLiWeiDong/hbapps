//
// Created by 李卫东 on 2019-02-19.
//
#include <hb/monitor_mail_plugin/monitor_mail_plugin_impl.h>
#include <hb/time/time.h>
#include <hb/send_mail_plugin/send_mail_plugin.h>

namespace hb{ namespace plugin {
        monitor_mail_plugin_impl::~monitor_mail_plugin_impl(){
            
        }
        void monitor_mail_plugin_impl::send_test_mail() {
                auto &mail_plugin = app().get_plugin<send_mail_plugin>();
                auto api = mail_plugin.get_api();
                api->send_mail(notify_mail_to_, "[monitor mail] test communication channel.", "monitor mail send test msg is ok !!!");
        }
        void monitor_mail_plugin_impl::loop() {
            auto self = shared_from_this();
            auto & io = app().get_io_service();
            deadline_updater_ = make_shared<boost::asio::deadline_timer>(io, boost::posix_time::seconds(30));
	    deadline_updater_->async_wait([self](const boost::system::error_code &ec){
                log_info<<"monitor mail loop is runing!";
                int cur_minutes = hb::time::hours_of_day()*60+hb::time::minutes_of_day();
                if(self->sended_msg_day_!=hb::time::day_of_month() && 
                        cur_minutes>=self->notify_time_ && 
                        cur_minutes<=self->notify_time_+5)
                {
                        log_info<<"cur_minutes:"<<cur_minutes<<" "<<self->notify_time_ ;
                        log_info<<"sended_msg_day:" << self->sended_msg_day_;
                        self->send_test_mail();
                        self->sended_msg_day_ = hb::time::day_of_month();
                }
                self->loop();
            });
        }
        void monitor_mail_plugin_impl::start() {
                send_test_mail();
                loop();
        }
       
} }