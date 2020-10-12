//
// Created by 李卫东 on 2019-02-26.
//
#pragma once

#include <memory>
#include <thread>
#include <functional>
#include <string>
#include <appbase/application.hpp>
#include <hb/log_plugin/log_plugin.h>
#include <hb/mysql_plugin/mysql_plugin.h>
#include <hb/grid_db_plugin/grid_db_plugin_impl.h>
#include <hb/grid_db_plugin/grid_db_plugin_api.h>

using namespace std;

namespace hb{ namespace plugin {
        using namespace appbase;
        
        class grid_db_plugin : public appbase::plugin<grid_db_plugin> {
        public:
            APPBASE_PLUGIN_REQUIRES((log_plugin)(mysql_plugin))
            grid_db_plugin();
            virtual ~grid_db_plugin();
            virtual void set_program_options(options_description&, options_description&) override;
            void plugin_initialize(const variables_map&);
            void plugin_startup();
            void plugin_shutdown();
            shared_ptr<grid_db_plugin_api> get_api(){
                return make_shared<grid_db_plugin_api>(my);
            }
        private:
            shared_ptr<grid_db_plugin_impl> my;
        };
}}