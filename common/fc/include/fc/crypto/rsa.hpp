#pragma once

#include <string>

namespace fc { namespace crypto {
    void create_rsa_key(std::string &strPri, std::string &strPub);
    std::string rsa_encrypt(const std::string &strPub, const std::string &plainText);
    std::string rsa_decrypt(const std::string &strPri, const std::string &cipherText);
    std::string sign_msg( const std::string& strPri, const std::string& msg );
    void verify_msg(const std::string& strPub, const std::string& msg, const std::string& sig);
    void sava_rsa_pem_pub(const std::string &pem_path, const std::string &strPub);
    void sava_rsa_pem_pri(const std::string &pem_path, const std::string &strPri, const std::string &pass = "");
    std::string read_rsa_pem_pub(const std::string &pem_path);
    std::string read_rsa_pem_pri(const std::string &pem_path, const std::string &pass = "");
} } // namespace fc
