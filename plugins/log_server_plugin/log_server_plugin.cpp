#include <hb/log_server_plugin/log_server_plugin.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <stdlib.h>
#include <vector>

namespace hb{ namespace plugin{
        static appbase::abstract_plugin& _log_server_plugin = app().register_plugin<log_server_plugin>();
        log_server_plugin::log_server_plugin(){

        }
        log_server_plugin::~log_server_plugin(){

        }
        void log_server_plugin::set_program_options(options_description& cli, options_description& cfg) {
                cfg.add_options()
                        ("log-server-udp-port", boost::program_options::value<int>()->default_value(514), "log-server-udp-port")
                        ("log-server-msg-after", boost::program_options::value<string>()->default_value("msg->"), "log-server-udp-port")
                        ;
            
        }
        void log_server_plugin::plugin_initialize(const variables_map& options) {
                log_info<<"log_server_plugin::plugin_initialize";
                my = make_shared<log_server_plugin_impl>(options.at( "log-server-udp-port" ).as<int>()); 
                my->msg_after(options.at( "log-server-msg-after" ).as<string>());             
        }
        void log_server_plugin::plugin_startup() {
                log_info<<"log_server_plugin::plugin_startup";
                my->start();
        }
        void log_server_plugin::plugin_shutdown() {
                log_info<<"log_server_plugin::plugin_shutdown";
                if(my)
                   my.reset();
        }
} }