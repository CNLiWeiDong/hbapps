#pragma once

#include <hb/error/exception.h>

namespace hb{ namespace plugin {
    using namespace hb::error;

    class monitor_price_exception : public hb_exception<monitor_price_exception> {
    public:
        monitor_price_exception():hb_exception<monitor_price_exception>(monitor_price_plugin_no, get_title(monitor_price_plugin_no)) {

        }
    };
} }