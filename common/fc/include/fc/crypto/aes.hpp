#pragma once
#include <boost/filesystem/path.hpp>
#include <fc/crypto/openssl.hpp>
#include <fc/crypto/sha256.hpp>
#include <fc/crypto/sha512.hpp>
#include <fc/types/types.hpp>
#include <vector>

namespace fc {

unsigned aes_encrypt(unsigned char *plaintext, int plaintext_len,
                     unsigned char *key, unsigned char *iv,
                     unsigned char *ciphertext);
unsigned aes_decrypt(unsigned char *ciphertext, int ciphertext_len,
                     unsigned char *key, unsigned char *iv,
                     unsigned char *plaintext);
unsigned aes_cfb_decrypt(unsigned char *ciphertext, int ciphertext_len,
                         unsigned char *key, unsigned char *iv,
                         unsigned char *plaintext);

std::vector<char> aes_encrypt(const fc::sha512 &key,
                              const std::vector<char> &plain_text);
std::vector<char> aes_decrypt(const fc::sha512 &key,
                              const std::vector<char> &cipher_text);

} // namespace fc
