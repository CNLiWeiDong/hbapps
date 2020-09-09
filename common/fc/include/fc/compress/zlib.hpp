#pragma once

#include <fc/types/types.hpp>
#include <string>

using namespace std;

namespace fc {

bytes zlib_compress(const bytes &in);
bytes zlib_decompress(const bytes &in);

} // namespace fc
