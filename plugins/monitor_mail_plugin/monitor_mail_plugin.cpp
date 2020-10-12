#include <hb/monitor_mail_plugin/monitor_mail_plugin.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <stdlib.h>
#include <vector>
#include <hb/monitor_mail_plugin/monitor_mail_error.h>

namespace hb{ namespace plugin{
        static appbase::abstract_plugin& _monitor_mail_plugin = app().register_plugin<monitor_mail_plugin>();
        monitor_mail_plugin::monitor_mail_plugin(){

        }
        monitor_mail_plugin::~monitor_mail_plugin(){

        }
        void monitor_mail_plugin::set_program_options(options_description& cli, options_description& cfg) {
                cfg.add_options()
                        ("monitor-mail-notify-to", boost::program_options::value<string>()->default_value("357879926@qq.com"), "monitor-mail-notify-to")
                        ("monitor-mail-notify-time", boost::program_options::value<string>()->default_value("8:10"), "monitor-mail-notify-time")
                        ;
            
        }
        void monitor_mail_plugin::plugin_initialize(const variables_map& options) {
                log_info<<"monitor_mail_plugin::plugin_initialize";
                my = make_shared<monitor_mail_plugin_impl>();
                my->notify_mail_to(options.at( "monitor-mail-notify-to" ).as<string>());
                string time_str = options.at( "monitor-mail-notify-time" ).as<string>();
                vector<string> props;
                boost::split(props, time_str, boost::is_any_of(":"));
                log_info<<"monitor-mail-notify-time:"<<time_str;
                log_info<<"monitor-mail-notify-time hours:"<<props[0]<<" minutes:"<< props[1];
                if(props.size()<2){
                        hb::plugin::monitor_mail_exception e;
                        e.msg("monitor-mail-notify-time error: %s", time_str);
                        hb_throw(e);
                }
                my->notify_time(stoi(props[0])*60+stoi(props[1]));
                
        }
        void monitor_mail_plugin::plugin_startup() {
                log_info<<"monitor_mail_plugin::plugin_startup";
                my->start();
        }
        void monitor_mail_plugin::plugin_shutdown() {
                log_info<<"monitor_mail_plugin::plugin_shutdown";
                if(my)
                   my.reset();
        }
} }