#pragma once

#include <string>

namespace hb { namespace crypto {
    std::string sha1(const std::string &msg);
    bool sha1_verify(const std::string &msg,const std::string &digest);

    std::string sha256(const std::string &msg);
    bool sha256_verify(const std::string &msg,const std::string &digest);

    std::string sha512(const std::string &msg);
    bool sha512_verify(const std::string &msg,const std::string &digest);
} } // namespace hb
