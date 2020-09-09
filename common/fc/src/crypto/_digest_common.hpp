#pragma once

/* Common stuff for cryptographic hashes
 */
// #include <stdlib.h>
// #include <string.h>
// #include <inttypes.h>
#include <cstring>
#include <string>

namespace fc {
namespace detail {
void shift_l(const char *in, char *out, std::size_t n, unsigned int i);
void shift_r(const char *in, char *out, std::size_t n, unsigned int i);
} // namespace detail
} // namespace fc
