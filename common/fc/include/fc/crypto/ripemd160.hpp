#pragma once

#include <fc/crypto/hex.hpp>
#include <fc/crypto/sha256.hpp>
#include <fc/crypto/sha512.hpp>
#include <openssl/ripemd.h>
#include <openssl/sha.h>
#include <string.h>
#include <vector>

namespace fc {
class sha512;
class sha256;

class ripemd160 {
public:
  ripemd160();
  explicit ripemd160(const string &hex_str);

  string str() const;
  explicit operator string() const;

  char *data() const;
  size_t data_size() const { return 160 / 8; }

  static ripemd160 hash(const fc::sha512 &h);
  static ripemd160 hash(const fc::sha256 &h);
  static ripemd160 hash(const char *d, uint32_t dlen);
  static ripemd160 hash(const string &);

  class encoder {
  public:
    encoder();
    ~encoder();

    void write(const char *d, uint32_t dlen);
    void put(char c) { write(&c, 1); }
    void reset();
    ripemd160 result();

  private:
    RIPEMD160_CTX ctx;
  };

  friend ripemd160 operator<<(const ripemd160 &h1, uint32_t i);
  friend bool operator==(const ripemd160 &h1, const ripemd160 &h2);
  friend bool operator!=(const ripemd160 &h1, const ripemd160 &h2);
  friend ripemd160 operator^(const ripemd160 &h1, const ripemd160 &h2);
  friend bool operator>=(const ripemd160 &h1, const ripemd160 &h2);
  friend bool operator>(const ripemd160 &h1, const ripemd160 &h2);
  friend bool operator<(const ripemd160 &h1, const ripemd160 &h2);

  uint32_t _hash[5];
};

} // namespace fc
