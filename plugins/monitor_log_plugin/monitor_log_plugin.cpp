#include <hb/monitor_log_plugin/monitor_log_plugin.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <stdlib.h>
#include <set>

namespace hb{ namespace plugin{
        static appbase::abstract_plugin& _monitor_log_plugin = app().register_plugin<monitor_log_plugin>();
        monitor_log_plugin::monitor_log_plugin(){

        }
        monitor_log_plugin::~monitor_log_plugin(){

        }
        void monitor_log_plugin::set_program_options(options_description& cli, options_description& cfg) {
                cfg.add_options()
                        ("trad-api-host", boost::program_options::value<string>()->default_value("api.huobi.pro"), "trad-api-host")
                        ("trad-api-port", boost::program_options::value<string>()->default_value("443"), "trad-api-port")
                        ("trad-api-access-key", boost::program_options::value<string>()->default_value("xxxx"), "trad-api-access-key")
                        ("trad-api-secret-key", boost::program_options::value<string>()->default_value("xxxx"), "trad-api-secret-key.")
                        ("trad-api-target-pair", boost::program_options::value<string>()->default_value("eosusdt"), "trad-api-target-pair")
                        ("trad-api-expired-seconds", boost::program_options::value<int>()->default_value(30), "trad-api-expired-seconds")
                        ("trad-api-cert-pem", boost::program_options::value<string>()->default_value("0"), "trad-api-cert-pem")
                        ("trad-api-url-query-pirce", boost::program_options::value<string>()->default_value("/market/detail/merged?symbol="), "trad-api-url-query-pirce GET")
                        ("trad-api-url-query-account", boost::program_options::value<string>()->default_value("/v1/account/accounts"), "trad-api-url-query-account GET")
                        ("trad-api-url-query-order", boost::program_options::value<string>()->default_value("/v1/order/orders/{order-id}"), "trad-api-url-query-order GET")
                        ("trad-api-url-query-order-client", boost::program_options::value<string>()->default_value("/v1/order/orders/getClientOrder"), "trad-api-url-query-order-client GET")
                        ("trad-api-url-cancel-order", boost::program_options::value<string>()->default_value("/v1/order/orders/{order-id}/submitcancel"), "trad-api-url-cancel-order POST")
                        ("trad-api-url-new-order", boost::program_options::value<string>()->default_value("/v1/order/orders/place"), "trad-api-url-new-order. POST")
                        ;
            
        }
        void monitor_log_plugin::plugin_initialize(const variables_map& options) {
                log_info<<"monitor_log_plugin::plugin_initialize";
                my = make_shared<monitor_log_plugin_impl>();
                
                
        }
        void monitor_log_plugin::plugin_startup() {
                log_info<<"monitor_log_plugin::plugin_startup";
                my->start();
        }
        void monitor_log_plugin::plugin_shutdown() {
                log_info<<"monitor_log_plugin::plugin_shutdown";
                if(my)
                   my.reset();
        }
} }