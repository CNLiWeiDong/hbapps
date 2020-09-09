//
// Created by 李卫东 on 2019-02-26.
//

#include <hb/mysql_plugin/mysql_plugin.h>


namespace hb{ namespace plugin{
        static appbase::abstract_plugin& _mysql_plugin = app().register_plugin<mysql_plugin>();
        mysql_plugin::mysql_plugin(){

        }
        mysql_plugin::~mysql_plugin(){

        }

        void mysql_plugin::set_program_options(options_description& cli, options_description& cfg) {
            cfg.add_options()
                    ("mysql-pool-max-size", bpo::value<int>()->default_value(32), "mysql connect pool max num");
        }
        void mysql_plugin::plugin_initialize(const variables_map& options) {
            log_info<<"mysql_plugin::plugin_initialize";
            my = make_shared<mysql_plugin_impl>();
            my->pool_max_size(options.at("mysql-pool-max-size").as<int>());
        }
        void mysql_plugin::plugin_startup() {
            log_info<<"mysql_plugin::plugin_startup";
        }
        void mysql_plugin::plugin_shutdown() {
            log_info<<"mysql_plugin::plugin_shutdown";
        }
    } }