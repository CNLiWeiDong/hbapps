#pragma once

#include <hb/error/exception.h>

namespace hb {  namespace sycn {
    using namespace hb::error;

    class hb_https_exception : public hb_exception<hb_https_exception> {
        public:
        hb_https_exception():hb_exception<hb_https_exception>(hb_https_no, get_title(hb_sync_no)) {

        }
    };
} }