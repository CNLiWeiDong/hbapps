//
// Created by 李卫东 on 2019-02-18.
//

#include "hb/thread_pool_plugin/thread_pool_plugin.h"


namespace hb{ namespace plugin{
        static appbase::abstract_plugin& _thread_pool_plugin = app().register_plugin<thread_pool_plugin>();
        thread_pool_plugin::thread_pool_plugin(){

        }
        thread_pool_plugin::~thread_pool_plugin(){

        }

        void thread_pool_plugin::set_program_options(options_description& cli, options_description& cfg) {
                cfg.add_options()
                        ("thread-pool-count", bpo::value<int>()->default_value(24),
                         "threads num in thread pool.");
        }
        void thread_pool_plugin::plugin_initialize(const variables_map& options) {
                log_info<<"thread_pool_plugin::plugin_initialize";
                my = make_shared<thread_pool_plugin_impl>(options.at("thread-pool-count").as<int>());
        }
        void thread_pool_plugin::plugin_startup() {
                log_info<<"thread_pool_plugin::plugin_startup";
        }
        void thread_pool_plugin::plugin_shutdown() {
                log_info<<"thread_pool_plugin::plugin_shutdown";
                if(my)
                   my.reset();
        }
} }