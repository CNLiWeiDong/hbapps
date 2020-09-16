#pragma once

#include <string>

namespace fc { namespace crypto {
    std::string cfb_aes_encrypt(const std::string &sKey, const std::string &plainText);
    std::string cfb_aes_encrypt(const std::string &plainText);
    std::string cfb_aes_decrypt(const std::string &sKey, const std::string &cipherText);
    std::string cfb_aes_decrypt(const std::string &cipherText);
} } // namespace fc
