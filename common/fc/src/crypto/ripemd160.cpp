#include "_digest_common.hpp"
#include <fc/crypto/ripemd160.hpp>

namespace fc {

ripemd160::ripemd160() { memset(_hash, 0, sizeof(_hash)); }
ripemd160::ripemd160(const string &hex_str) {
  auto bytes_written = fc::from_hex(hex_str, (char *)_hash, sizeof(_hash));
  if (bytes_written < sizeof(_hash))
    memset((char *)_hash + bytes_written, 0, (sizeof(_hash) - bytes_written));
}

string ripemd160::str() const {
  return fc::to_hex((char *)_hash, sizeof(_hash));
}
ripemd160::operator string() const { return str(); }

char *ripemd160::data() const { return (char *)&_hash[0]; }

ripemd160::encoder::~encoder() {}
ripemd160::encoder::encoder() {
  memset((char *)&ctx, 0, sizeof(ctx));
  reset();
}

ripemd160 ripemd160::hash(const fc::sha512 &h) {
  return hash((const char *)&h, sizeof(h));
}
ripemd160 ripemd160::hash(const fc::sha256 &h) {
  return hash((const char *)&h, sizeof(h));
}
ripemd160 ripemd160::hash(const char *d, uint32_t dlen) {
  encoder e;
  e.write(d, dlen);
  return e.result();
}
ripemd160 ripemd160::hash(const string &s) { return hash(s.c_str(), s.size()); }

void ripemd160::encoder::write(const char *d, uint32_t dlen) {
  RIPEMD160_Update(&ctx, d, dlen);
}
ripemd160 ripemd160::encoder::result() {
  ripemd160 h;
  RIPEMD160_Final((uint8_t *)h.data(), &ctx);
  return h;
}
void ripemd160::encoder::reset() { RIPEMD160_Init(&ctx); }

ripemd160 operator<<(const ripemd160 &h1, uint32_t i) {
  ripemd160 result;
  fc::detail::shift_l(h1.data(), result.data(), result.data_size(), i);
  return result;
}
ripemd160 operator^(const ripemd160 &h1, const ripemd160 &h2) {
  ripemd160 result;
  result._hash[0] = h1._hash[0] ^ h2._hash[0];
  result._hash[1] = h1._hash[1] ^ h2._hash[1];
  result._hash[2] = h1._hash[2] ^ h2._hash[2];
  result._hash[3] = h1._hash[3] ^ h2._hash[3];
  result._hash[4] = h1._hash[4] ^ h2._hash[4];
  return result;
}
bool operator>=(const ripemd160 &h1, const ripemd160 &h2) {
  return memcmp(h1._hash, h2._hash, sizeof(h1._hash)) >= 0;
}
bool operator>(const ripemd160 &h1, const ripemd160 &h2) {
  return memcmp(h1._hash, h2._hash, sizeof(h1._hash)) > 0;
}
bool operator<(const ripemd160 &h1, const ripemd160 &h2) {
  return memcmp(h1._hash, h2._hash, sizeof(h1._hash)) < 0;
}
bool operator!=(const ripemd160 &h1, const ripemd160 &h2) {
  return memcmp(h1._hash, h2._hash, sizeof(h1._hash)) != 0;
}
bool operator==(const ripemd160 &h1, const ripemd160 &h2) {
  return memcmp(h1._hash, h2._hash, sizeof(h1._hash)) == 0;
}

} // namespace fc
