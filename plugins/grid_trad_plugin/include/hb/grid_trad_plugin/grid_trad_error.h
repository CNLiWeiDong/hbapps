#pragma once

#include <hb/error/exception.h>

namespace hb{ namespace plugin {
    using namespace hb::error;

    class grid_trad_exception : public hb_exception<grid_trad_exception> {
    public:
        grid_trad_exception():hb_exception<grid_trad_exception>(grid_trad_plugin_no, get_title(grid_trad_plugin_no)) {

        }
    };
} }