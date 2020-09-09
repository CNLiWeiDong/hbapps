#include "fc/logging/logging.h"
#include <fc/crypto/hex.hpp>

namespace fc {

uint8_t from_hex(char c) {
  if (c >= '0' && c <= '9')
    return c - '0';
  if (c >= 'a' && c <= 'f')
    return c - 'a' + 10;
  if (c >= 'A' && c <= 'F')
    return c - 'A' + 10;
  LOG_FATAL("Invalid hex character %c", c);
  return 0;
}

std::string to_hex(const char *d, uint32_t s) {
  std::string r;
  const char *to_hex = "0123456789abcdef";
  uint8_t *c = (uint8_t *)d;
  for (uint32_t i = 0; i < s; ++i)
    (r += to_hex[(c[i] >> 4)]) += to_hex[(c[i] & 0x0f)];
  return r;
}

size_t from_hex(const string &hex_str, char *out_data, size_t out_data_len) {
  string::const_iterator i = hex_str.begin();
  uint8_t *out_pos = (uint8_t *)out_data;
  uint8_t *out_end = out_pos + out_data_len;
  while (i != hex_str.end() && out_end != out_pos) {
    *out_pos = from_hex(*i) << 4;
    ++i;
    if (i != hex_str.end()) {
      *out_pos |= from_hex(*i);
      ++i;
    }
    ++out_pos;
  }
  return out_pos - (uint8_t *)out_data;
}
std::vector<char> from_hex(const string &hex_str) {
  vector<char> r(hex_str.size());
  auto l = from_hex(hex_str, r.data(), hex_str.size());
  r.resize(l);
  return r;
}
std::string to_hex(const std::vector<char> &data) {
  if (data.size())
    return to_hex(data.data(), data.size());
  return "";
}

} // namespace fc
