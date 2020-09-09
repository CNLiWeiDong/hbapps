#pragma once
#include <openssl/sha.h>
#include <string>

using namespace std;
namespace fc {

class sha1 {
public:
  sha1();
  explicit sha1(const string &hex_str);

  string str() const;
  operator string() const;

  char *data() const;
  size_t data_size() const { return 20; }

  static sha1 hash(const char *d, uint32_t dlen);
  static sha1 hash(const string &);

  class encoder {
  public:
    encoder();
    ~encoder();

    void write(const char *d, uint32_t dlen);
    void put(char c) { write(&c, 1); }
    void reset();
    sha1 result();

  private:
    SHA_CTX ctx;
  };

  friend sha1 operator<<(const sha1 &h1, uint32_t i);
  friend bool operator==(const sha1 &h1, const sha1 &h2);
  friend bool operator!=(const sha1 &h1, const sha1 &h2);
  friend sha1 operator^(const sha1 &h1, const sha1 &h2);
  friend bool operator>=(const sha1 &h1, const sha1 &h2);
  friend bool operator>(const sha1 &h1, const sha1 &h2);
  friend bool operator<(const sha1 &h1, const sha1 &h2);

  uint32_t _hash[5];
};

} // namespace fc