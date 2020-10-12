#pragma once

#include <hb/error/exception.h>

namespace hb{ namespace plugin {
    using namespace hb::error;

    class monitor_mail_exception : public hb_exception<monitor_mail_exception> {
    public:
        monitor_mail_exception():hb_exception<monitor_mail_exception>(monitor_mail_plugin_no, get_title(monitor_mail_plugin_no)) {

        }
    };
} }