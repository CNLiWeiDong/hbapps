//
// Created by 李卫东 on 2019-02-19.
//
#pragma once


#include <functional>
#include <string>
#include <appbase/application.hpp>
#include "hb/log_plugin/log_plugin.h"
#include <hb/grid_trad_plugin/grid_trad_plugin_impl.h>
#include <hb/grid_db_plugin/grid_db_plugin.h>
#include <hb/send_mail_plugin/send_mail_plugin.h>
#include <hb/trad_api_plugin/trad_api_plugin.h>



namespace hb{ namespace plugin {
    using namespace std;
    using namespace appbase;
    class grid_trad_plugin_api{
    public:
        grid_trad_plugin_api(shared_ptr<grid_trad_plugin_impl> _impl):impl(_impl){}
    private:
        shared_ptr<grid_trad_plugin_impl> impl;
    };
    class grid_trad_plugin : public appbase::plugin<grid_trad_plugin> {
        public:
            APPBASE_PLUGIN_REQUIRES((log_plugin)(grid_db_plugin)(send_mail_plugin)(trad_api_plugin))
            grid_trad_plugin();
            virtual ~grid_trad_plugin();
            virtual void set_program_options(options_description&, options_description&) override;
            void plugin_initialize(const variables_map&);
            void plugin_startup();
            void plugin_shutdown();
            shared_ptr<grid_trad_plugin_api> get_api(){
                return make_shared<grid_trad_plugin_api>(my);
            }
        private:
            shared_ptr<grid_trad_plugin_impl> my;
    };
}}
