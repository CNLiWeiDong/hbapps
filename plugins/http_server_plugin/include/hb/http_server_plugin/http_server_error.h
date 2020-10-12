#pragma once

#include <hb/error/exception.h>

namespace hb{ namespace plugin {
    using namespace hb::error;

    class http_server_exception : public hb_exception<http_server_exception> {
    public:
        http_server_exception():hb_exception<http_server_exception>(http_server_plugin_no, get_title(http_server_plugin_no)) {

        }
    };
} }