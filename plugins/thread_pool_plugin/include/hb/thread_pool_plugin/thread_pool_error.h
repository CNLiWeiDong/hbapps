#pragma once

#include <hb/error/exception.h>

namespace hb{ namespace plugin {
    using namespace hb::error;

    class thread_pool_exception : public hb_exception<thread_pool_exception> {
    public:
        thread_pool_exception():hb_exception<thread_pool_exception>(thread_pool_plugin_no, get_title(thread_pool_plugin_no)) {

        }
    };
} }