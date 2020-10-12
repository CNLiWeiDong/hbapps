#pragma once

#include <hb/error/exception.h>

namespace hb{ namespace plugin {
    using namespace hb::error;

    class send_mail_exception : public hb_exception<send_mail_exception> {
    public:
        send_mail_exception():hb_exception<send_mail_exception>(send_mail_plugin_no, get_title(send_mail_plugin_no)) {

        }
    };
} }