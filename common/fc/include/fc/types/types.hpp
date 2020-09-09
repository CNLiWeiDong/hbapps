#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <string>
#include <vector>

using namespace std;

namespace fc {

typedef boost::multiprecision::uint128_t uint128_t;
using bytes = vector<char>;

} // namespace fc