//
// Created by 李卫东 on 2019-02-18.
//

#include "hb/log_plugin/log_plugin.h"


namespace hb{ namespace plugin{
        static appbase::abstract_plugin& _log_plugin = app().register_plugin<log_plugin>();

        log_plugin::log_plugin(){

        }
        log_plugin::~log_plugin(){

        }

        void log_plugin::set_program_options(options_description& cli, options_description& cfg) {

        }
        void log_plugin::plugin_initialize(const variables_map& options) {
                log_info<<"log_plugin::plugin_initialize";
                hb::log::initialize_config(app().get_logging_conf());
        }
        void log_plugin::plugin_startup() {
                log_info<<"log_plugin::plugin_startup";
        }
        void log_plugin::plugin_shutdown() {
                log_info<<"log_plugin::plugin_shutdown";
        }
    } }