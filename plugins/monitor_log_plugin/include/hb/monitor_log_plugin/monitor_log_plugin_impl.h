//
// Created by 李卫东 on 2019-02-18.
//
#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <string>
#include <atomic>
#include <hb/log/log.h>
#include <appbase/application.hpp>

namespace hb{ namespace plugin {
    using namespace std;
    using namespace appbase;

    class monitor_log_plugin_impl: public std::enable_shared_from_this<monitor_log_plugin_impl>{
    public:
        void start();
    public:
        monitor_log_plugin_impl() {

        }
        ~monitor_log_plugin_impl();
    };
} }
