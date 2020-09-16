#pragma once

#include <string>

namespace fc { namespace crypto {
    std::string md5(const std::string &msg);
    bool md5_verify(const std::string &msg,const std::string &digest);
} } // namespace fc
