#pragma once

#include <hb/error/exception.h>

namespace hb{ namespace plugin {
    using namespace hb::error;

    class grid_db_exception : public hb_exception<grid_db_exception> {
    public:
        grid_db_exception():hb_exception<grid_db_exception>(grid_db_plugin_no, get_title(grid_db_plugin_no)) {

        }
    };
} }