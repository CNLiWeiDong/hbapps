//
// Created by 李卫东 on 2019-02-18.
//

#include "hb/logging_plugin/logging_plugin.h"


namespace hb{ namespace plugin{
        static appbase::abstract_plugin& _log_plugin = app().register_plugin<logging_plugin>();

        logging_plugin::logging_plugin(){

        }
        logging_plugin::~logging_plugin(){

        }

        void logging_plugin::set_program_options(options_description& cli, options_description& cfg) {

        }
        void logging_plugin::plugin_initialize(const variables_map& options) {
                log_info<<"logging_plugin::plugin_initialize";
                fc::initialize_config(app().get_logging_conf());
        }
        void logging_plugin::plugin_startup() {
                log_info<<"logging_plugin::plugin_startup";
        }
        void logging_plugin::plugin_shutdown() {
                log_info<<"logging_plugin::plugin_shutdown";
        }
    } }