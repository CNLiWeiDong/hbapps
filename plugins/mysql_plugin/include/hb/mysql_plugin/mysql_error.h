#pragma once

#include <hb/error/exception.h>

namespace hb{ namespace plugin {
    using namespace hb::error;

    class mysql_exception : public hb_exception<mysql_exception> {
    public:
        mysql_exception():hb_exception<mysql_exception>(mysql_plugin_no, get_title(mysql_plugin_no)) {

        }
    };
} }