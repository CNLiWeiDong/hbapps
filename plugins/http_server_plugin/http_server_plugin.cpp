#include <hb/http_server_plugin/http_server_plugin.h>
#include <hb/http_server_plugin/http_server_error.h>

namespace hb{ namespace plugin{
        static appbase::abstract_plugin& _http_server_plugin = app().register_plugin<http_server_plugin>();
        http_server_plugin::http_server_plugin(){

        }
        http_server_plugin::~http_server_plugin(){

        }
        void http_server_plugin::set_program_options(options_description& cli, options_description& cfg) {
                cfg.add_options()
                        ("http-host", boost::program_options::value<string>()->default_value("127.0.0.1"), "the http server ip")
                        ("http-port", boost::program_options::value<int>()->default_value(8888), "the http server port")
                        ("http-body-size", bpo::value<uint64_t>()->default_value(1024*1024), "The maximum body size in bytes allowed for incoming requests")
                        ("http-expires-seconds", bpo::value<uint32_t>()->default_value(30), "The connect keep live seconds time")
                        // ("http-enable-path", bpo::value<vector<string>>(), "The connect keep live seconds time")
                        ("https-enable", boost::program_options::value<bool>()->default_value(false), "If set to false, then the http server is valid")
                        ("https-certificate-file", bpo::value<string>(),
                            "Filename with the certificate chain to present on https connections. PEM format. File path in config-dir.")
                        ("https-private-file", bpo::value<string>(),
                            "Filename with https private key in PEM format. File path in config-dir.")
                        ("https-private-password", bpo::value<string>()->default_value(""),
                            "password with the private key. File path in config-dir.");

        }
        void http_server_plugin::plugin_initialize(const variables_map& options) {
                log_info<<"http_server_plugin::plugin_initialize";
                if(
                    options.at( "https-enable" ).as<bool>() &&
                    (
                        !( options.count( "https-certificate-file" ) && options.at( "https-certificate-file" ).as<string>().length()) ||
                        !( options.count( "https-private-file" ) && options.at( "https-private-file" ).as<string>().length())
                    )
                )
                {
                    http_server_exception e;
                    e.msg("https server need certificate-file and private-file in config-dir path!");
                    hb_throw(e);
                }
                string certificate;
                if( options.count( "https-certificate-file" ) && options.at( "https-certificate-file" ).as<string>().length())
                    certificate = options.at( "https-certificate-file" ).as<string>();
                string private_key;
                if( options.count( "https-private-file" ) && options.at( "https-private-file" ).as<string>().length())
                    private_key = options.at( "https-private-file" ).as<string>();
                
                http_options http_op = {
                    https_enable:         options.at( "https-enable" ).as<bool>(),
                    host:           options.at( "http-host" ).as<string>(),
                    port:           options.at( "http-port" ).as<int>(),
                    body_size:      options.at( "http-body-size" ).as<uint64_t>(),
                    expires_seonds: options.at( "http-expires-seconds" ).as<uint32_t>(),
                    certificate_file:   certificate,
                    private_file:       private_key,
                    private_password:   options.at( "https-private-password" ).as<string>()
                };
                my = make_shared<http_server_plugin_impl>(http_op);
        }
        void http_server_plugin::plugin_startup() {
                log_info<<"http_server_plugin::plugin_startup";
                my->start();
        }
        void http_server_plugin::plugin_shutdown() {
                log_info<<"http_server_plugin::plugin_shutdown";
                if(my)
                   my.reset();
        }
} }