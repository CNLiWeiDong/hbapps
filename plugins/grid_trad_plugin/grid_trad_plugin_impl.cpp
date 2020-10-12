//
// Created by 李卫东 on 2019-02-19.
//
#include <hb/grid_trad_plugin/grid_trad_plugin_impl.h>
#include <hb/time/time.h>

namespace hb{ namespace plugin {
        grid_trad_plugin_impl::~grid_trad_plugin_impl(){
        }
        
        void grid_trad_plugin_impl::deal_loop(){
            log_info<<"【grid_trad_plugin_impl::deal_loop】";
            hb_try
                log_info<<"delvier_order  ========>>>>>>>>";
                auto dealing_segs = delvier_order_->deal();
                log_info<<"new_order      ========>>>>>>>>";
                new_order_->deal(dealing_segs);
            hb_catch([](const auto &e){
                log_throw("deal_loop throw.", e);
            })
            loop();
        }
        void grid_trad_plugin_impl::loop() {
            auto self = shared_from_this();
            auto & io = app().get_io_service();
            deadline_updater_ = make_shared<boost::asio::deadline_timer>(io, boost::posix_time::seconds(intervals_seconds_));
	        deadline_updater_->async_wait([self](const boost::system::error_code &ec){
                self->deal_loop();
            });
        }
        void grid_trad_plugin_impl::start(){
            deal_loop();
        }
        
} }