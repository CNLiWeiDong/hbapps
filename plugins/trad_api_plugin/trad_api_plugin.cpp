#include <hb/trad_api_plugin/trad_api_plugin.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <stdlib.h>
#include <set>

namespace hb{ namespace plugin{
        static appbase::abstract_plugin& _trad_api_plugin = app().register_plugin<trad_api_plugin>();
        trad_api_plugin::trad_api_plugin(){

        }
        trad_api_plugin::~trad_api_plugin(){

        }
        void trad_api_plugin::set_program_options(options_description& cli, options_description& cfg) {
                cli.add_options()
                        ("secret-key-password,p", bpo::value<std::string>()->default_value( "no" ), "The password use to decrypt secret-key.");
                cfg.add_options()
                        ("trad-api-host", boost::program_options::value<string>()->default_value("api.huobi.pro"), "trad-api-host")
                        ("trad-api-port", boost::program_options::value<string>()->default_value("443"), "trad-api-port")
                        ("trad-api-access-key", boost::program_options::value<string>()->default_value("xxxx"), "trad-api-access-key")
                        ("trad-api-secret-key", boost::program_options::value<string>()->default_value("xxxx"), "trad-api-secret-key.")
                        ("trad-api-target-pair", boost::program_options::value<string>()->default_value("eosusdt"), "trad-api-target-pair")
                        ("trad-api-expired-seconds", boost::program_options::value<int>()->default_value(30), "trad-api-expired-seconds")
                        ("trad-api-sleep-sleep_milliseconds", boost::program_options::value<int>()->default_value(200), "trad-api-sleep-sleep_milliseconds")
                        ("trad-api-cert-pem", boost::program_options::value<string>()->default_value("0"), "trad-api-cert-pem")
                        ("trad-api-url-query-pirce", boost::program_options::value<string>()->default_value("/market/detail/merged?symbol="), "trad-api-url-query-pirce GET")
                        ("trad-api-url-query-account", boost::program_options::value<string>()->default_value("/v1/account/accounts"), "trad-api-url-query-account GET")
                        ("trad-api-url-query-order", boost::program_options::value<string>()->default_value("/v1/order/orders/{order-id}"), "trad-api-url-query-order GET")
                        ("trad-api-url-query-order-client", boost::program_options::value<string>()->default_value("/v1/order/orders/getClientOrder"), "trad-api-url-query-order-client GET")
                        ("trad-api-url-cancel-order", boost::program_options::value<string>()->default_value("/v1/order/orders/{order-id}/submitcancel"), "trad-api-url-cancel-order POST")
                        ("trad-api-url-new-order", boost::program_options::value<string>()->default_value("/v1/order/orders/place"), "trad-api-url-new-order. POST")
                        ;
            
        }
        void trad_api_plugin::plugin_initialize(const variables_map& options) {
                log_info<<"trad_api_plugin::plugin_initialize";
                my = make_shared<trad_api_plugin_impl>();
                trad_api_data_type data =  {
                        host: options.at( "trad-api-host" ).as<string>(),
                        port: options.at( "trad-api-port" ).as<string>(),
                        access_key: options.at( "trad-api-access-key" ).as<string>(),
                        secret_key: options.at( "trad-api-secret-key" ).as<string>(),
                        target_pair: options.at( "trad-api-target-pair" ).as<string>(),
                        expired_seconds: options.at( "trad-api-expired-seconds" ).as<int>(),
                        sleep_milliseconds: options.at( "trad-api-sleep-sleep_milliseconds" ).as<int>(),
                        cert_pem: options.at( "trad-api-cert-pem" ).as<string>(),
                        url_query_pirce: options.at( "trad-api-url-query-pirce" ).as<string>(),
                        url_query_account: options.at( "trad-api-url-query-account" ).as<string>(),
                        url_query_order: options.at( "trad-api-url-query-order" ).as<string>(),
                        url_query_order_client: options.at( "trad-api-url-query-order-client" ).as<string>(),
                        url_cancel_order: options.at( "trad-api-url-cancel-order" ).as<string>(),
                        url_new_order: options.at( "trad-api-url-new-order" ).as<string>()
                };
                const string pwd_of_secret_key = options.at( "secret-key-password" ).as<string>();
                cout << pwd_of_secret_key;
                log_info<<"trad_api_plugin::plugin_initialize2";
                my->data(std::move(data));
                log_info<<"trad_api_plugin::plugin_initialize [end]";
                
        }
        void trad_api_plugin::plugin_startup() {
                log_info<<"trad_api_plugin::plugin_startup";
                my->start();
        }
        void trad_api_plugin::plugin_shutdown() {
                log_info<<"trad_api_plugin::plugin_shutdown";
                if(my)
                   my.reset();
        }
} }