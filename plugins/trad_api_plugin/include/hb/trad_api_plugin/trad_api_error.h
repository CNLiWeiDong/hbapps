#pragma once

#include <hb/error/exception.h>

namespace hb{ namespace plugin {
    using namespace hb::error;

    class trad_api_exception : public hb_exception<trad_api_exception> {
    public:
        trad_api_exception():hb_exception<trad_api_exception>(trad_api_plugin_no, get_title(trad_api_plugin_no)) {

        }
    };
} }