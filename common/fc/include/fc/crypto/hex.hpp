#pragma once
#include <string>
#include <vector>

using namespace std;
namespace fc {
uint8_t from_hex(char c);
string to_hex(const char *d, uint32_t s);
std::string to_hex(const std::vector<char> &data);

/**
 *  @return the number of bytes decoded
 */
size_t from_hex(const string &hex_str, char *out_data, size_t out_data_len);
std::vector<char> from_hex(const string &hex_str);
} // namespace fc
