#include <hb/send_mail_plugin/send_mail_plugin.h>
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <stdlib.h>
#include <set>

namespace hb{ namespace plugin{
        static appbase::abstract_plugin& _send_mail_plugin = app().register_plugin<send_mail_plugin>();
        send_mail_plugin::send_mail_plugin(){

        }
        send_mail_plugin::~send_mail_plugin(){

        }
        void send_mail_plugin::set_program_options(options_description& cli, options_description& cfg) {
                cfg.add_options()
                        ("mail-login-user", boost::program_options::value<string>()->default_value("test@126.com"), "the login user of send mail.")
                        ("mail-login-pass", boost::program_options::value<string>()->default_value("123456"), "the login user password of send mail.")
                        ("mail-server-host", boost::program_options::value<string>()->default_value("smtp.126.com"), "the mail server host.")
                        ("mail-server-port", boost::program_options::value<string>()->default_value("25"), "he mail server port.");
            
        }
        void send_mail_plugin::plugin_initialize(const variables_map& options) {
                log_info<<"send_mail_plugin::plugin_initialize";
                my = make_shared<send_mail_plugin_impl>();
                my->host(options.at( "mail-server-host" ).as<string>());
                my->port(options.at( "mail-server-port" ).as<string>());
                my->user(options.at( "mail-login-user" ).as<string>());
                my->pass(options.at( "mail-login-pass" ).as<string>());
        }
        void send_mail_plugin::plugin_startup() {
                log_info<<"send_mail_plugin::plugin_startup";
        }
        void send_mail_plugin::plugin_shutdown() {
                log_info<<"send_mail_plugin::plugin_shutdown";
                if(my)
                   my.reset();
        }
} }