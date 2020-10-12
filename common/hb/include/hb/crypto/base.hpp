#pragma once
#include <string>

namespace hb { namespace crypto {
    
    std::string base64(const std::string &str);
    std::string base64_str(const std::string &base64_str);
    std::string base64url(const std::string &str);
    std::string base64url_str(const std::string &base64url_str);
    std::string base32(const std::string &str);
    std::string base32_str(const std::string &base32_str);
} } // namespace hb
