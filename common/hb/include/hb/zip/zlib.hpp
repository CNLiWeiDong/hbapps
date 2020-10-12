#pragma once

#include <hb/types/types.hpp>
#include <string>

using namespace std;

namespace hb { namespace zip { 

hb::type::bytes zlib_compress(const hb::type::bytes &in);
hb::type::bytes zlib_decompress(const hb::type::bytes &in);

} } // namespace hb
