#pragma once

#include <hb/error/exception.h>

namespace hb{ namespace plugin {
    using namespace hb::error;

    class log_exception : public hb_exception<log_exception> {
    public:
        log_exception():hb_exception<log_exception>(log_plugin_no, get_title(log_plugin_no)) {

        }
    };
} }