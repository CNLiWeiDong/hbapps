#pragma once
#include <string>
#include <vector>

using namespace std;
namespace fc {
std::string rsa_pub_encrypt(const std::string &clearText,
                            const std::string &pubKey);
std::string rsa_pri_decrypt(const std::string &cipherText,
                            const std::string &priKey, const char *PASS);
} // namespace fc
