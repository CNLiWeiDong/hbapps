//
// Created by 李卫东 on 2019-08-21.
//
#include "openssl/rsa.h"
#include "fc/crypto/hex.hpp"
#include "fc/logging/logging.h"
#include "openssl/pem.h"
#include <fc/crypto/rsa.hpp>
#include <string>

using namespace std;

namespace fc {

// 公钥加密
std::string rsa_pub_encrypt(const std::string &clearText,
                            const std::string &pubKey) {
  std::string strRet;
  BIO *keybio = BIO_new_mem_buf((unsigned char *)pubKey.c_str(), -1);
  if (!keybio) {
    LOG_FATAL("BIO_new_mem_buf publicKey error");
  }
  RSA *rsa = nullptr;
  PEM_read_bio_RSA_PUBKEY(keybio, &rsa, NULL, NULL);
  if (!rsa) {
    BIO_free_all(keybio);
    LOG_FATAL("rsa read public key error");
  }

  int len = RSA_size(rsa);
  char *encryptedText = (char *)malloc(len + 1);
  if (!encryptedText) {
    BIO_free_all(keybio);
    RSA_free(rsa);
    LOG_FATAL("malloc memory error");
  }
  memset(encryptedText, 0, len + 1);

  // 加密函数
  int ret = RSA_public_encrypt(
      clearText.length(), (const unsigned char *)clearText.c_str(),
      (unsigned char *)encryptedText, rsa, RSA_PKCS1_PADDING);
  if (ret >= 0)
    strRet = std::string(encryptedText, ret);
  // 释放内存
  free(encryptedText);
  BIO_free_all(keybio);
  RSA_free(rsa);
  if (ret < 0) {
    LOG_FATAL("rsa RSA_public_encrypt error");
  }
  return strRet;
}

// 私钥解密
std::string rsa_pri_decrypt(const std::string &cipherText,
                            const std::string &priKey, const char *PASS) {
  std::string strRet;

  BIO *keybio;
  keybio = BIO_new_mem_buf((unsigned char *)priKey.c_str(), -1);
  if (!keybio) {
    LOG_FATAL("BIO_new_mem_buf privateKey error");
  }
  OpenSSL_add_all_algorithms(); //密钥有经过口令加密需要这个函数
  RSA *rsa = nullptr;
  rsa = PEM_read_bio_RSAPrivateKey(keybio, &rsa, NULL,
                                   (char *)PASS); // (char *)PASS ???
  if (!rsa) {
    BIO_free_all(keybio);
    LOG_FATAL("read private key error");
  }

  int len = RSA_size(rsa);
  char *decryptedText = (char *)malloc(len + 1);
  if (!decryptedText) {
    BIO_free_all(keybio);
    RSA_free(rsa);
    LOG_FATAL("malloc memory error");
  }
  memset(decryptedText, 0, len + 1);

  // 解密函数
  int ret = RSA_private_decrypt(
      cipherText.length(), (const unsigned char *)cipherText.c_str(),
      (unsigned char *)decryptedText, rsa, RSA_PKCS1_PADDING);
  if (ret >= 0)
    strRet = std::string(decryptedText, ret);

  // 释放内存
  free(decryptedText);
  BIO_free_all(keybio);
  RSA_free(rsa);
  if (ret < 0) {
    LOG_FATAL("RSA_private_decrypt error");
  }
  return strRet;
}

} // namespace fc
