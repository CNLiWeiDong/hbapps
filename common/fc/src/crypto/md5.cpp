//
// Created by 李卫东 on 2019-08-21.
//

#include "openssl/md5.h"
#include "fc/crypto/hex.hpp"
#include "fc/logging/logging.h"
#include <fc/crypto/md5.hpp>
#include <string>

using namespace std;

namespace fc {

string md5(const std::string &src) {
  // 调用md5哈希
  unsigned char mdStr[32] = {0};
  MD5((const unsigned char *)src.c_str(), src.length(), mdStr);
  return fc::to_hex((const char *)mdStr, 32);
}

} // namespace fc
