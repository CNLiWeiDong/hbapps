//
// Created by 李卫东 on 2019-02-18.
//
#include <boost/filesystem/path.hpp> 
#include <boost/filesystem/operations.hpp>
#include "appbase/application.hpp"
#include "hb/log_plugin/log_plugin.h"
#include "hb/monitor_mail_plugin/monitor_mail_plugin.h"

using namespace appbase;
using namespace hb::plugin;


int main(int argc,char **argv){
    auto exePath = boost::filesystem::initial_path<boost::filesystem::path>();
    app().set_default_config_dir(exePath/"config");
    app().set_default_data_dir(exePath/"data");
    if(!app().initialize<monitor_mail_plugin>(argc, argv))
        return 1;
    printf("app version %s\n",app().version_string().c_str());
    printf("app config directory is %s\n",app().config_dir().c_str());
    printf("app using config file %s\n",app().full_config_file_path().string().c_str());
    printf("app using log config file %s\n",app().get_logging_conf().string().c_str());
    printf("app data directory is %s\n",app().data_dir().string().c_str());
    app().startup();
    app().exec();
   return 0;
}