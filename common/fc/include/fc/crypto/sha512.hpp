#pragma once
#include <openssl/sha.h>
#include <string>

using namespace std;
namespace fc {

class sha512 {
public:
  sha512();
  explicit sha512(const string &hex_str);

  string str() const;
  operator string() const;

  char *data() const;
  size_t data_size() const { return 512 / 8; }

  static sha512 hash(const char *d, uint32_t dlen);
  static sha512 hash(const string &);

  class encoder {
  public:
    encoder();
    ~encoder();

    void write(const char *d, uint32_t dlen);
    void put(char c) { write(&c, 1); }
    void reset();
    sha512 result();

  private:
    SHA512_CTX ctx;
  };

  friend sha512 operator<<(const sha512 &h1, uint32_t i);
  friend bool operator==(const sha512 &h1, const sha512 &h2);
  friend bool operator!=(const sha512 &h1, const sha512 &h2);
  friend sha512 operator^(const sha512 &h1, const sha512 &h2);
  friend bool operator>=(const sha512 &h1, const sha512 &h2);
  friend bool operator>(const sha512 &h1, const sha512 &h2);
  friend bool operator<(const sha512 &h1, const sha512 &h2);

  uint64_t _hash[8];
};

} // namespace fc
