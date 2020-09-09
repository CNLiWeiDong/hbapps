//
// Created by 李卫东 on 2019-02-26.
//

#include <hb/grid_db_plugin/grid_db_plugin.h>


namespace hb{ namespace plugin{
        static appbase::abstract_plugin& _grid_db_plugin = app().register_plugin<grid_db_plugin>();
        grid_db_plugin::grid_db_plugin(){

        }
        grid_db_plugin::~grid_db_plugin(){

        }

        void grid_db_plugin::set_program_options(options_description& cli, options_description& cfg) {
            cfg.add_options()
                    ("grid_db_host", bpo::value<string>()->default_value("127.0.0.1"), "grid_db connect host")
                    ("grid_db_port", bpo::value<int>()->default_value(3306), "grid_db db port")
                    ("grid_db_name", bpo::value<string>()->default_value("eos_trad"), "grid_db db name")
                    ("grid_db_user", bpo::value<string>()->default_value("eos_trad"), "grid_db db user")
                    ("grid_db_pass", bpo::value<string>()->default_value("123456"), "grid_db db password")
                    ;
        }
        void grid_db_plugin::plugin_initialize(const variables_map& options) {
            log_info<<"grid_db_plugin::plugin_initialize";
            my = make_shared<grid_db_plugin_impl>();
            my->set_db_info(
                options.at("grid_db_host").as<string>(),
                options.at("grid_db_name").as<string>(),
                options.at("grid_db_user").as<string>(),
                options.at("grid_db_pass").as<string>(),
                options.at("grid_db_port").as<int>()
            );
        }
        void grid_db_plugin::plugin_startup() {
            log_info<<"grid_db_plugin::plugin_startup";
        }
        void grid_db_plugin::plugin_shutdown() {
            log_info<<"grid_db_plugin::plugin_shutdown";
        }
    } }