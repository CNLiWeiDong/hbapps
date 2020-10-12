#pragma once

#include <hb/error/exception.h>

namespace hb{ namespace plugin {
    using namespace hb::error;

    class monitor_log_exception : public hb_exception<monitor_log_exception> {
    public:
        monitor_log_exception():hb_exception<monitor_log_exception>(monitor_log_plugin_no, get_title(monitor_log_plugin_no)) {

        }
    };
} }