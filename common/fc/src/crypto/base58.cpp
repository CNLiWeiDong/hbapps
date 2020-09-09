// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2012 The Bitcoin Developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Why base-58 instead of standard base-64 encoding?
// - Don't want 0OIl characters that look the same in some fonts and
//      could be used to create visually identical looking account numbers.
// - A string with non-alphanumeric characters is not as easily accepted as an
// account number.
// - E-mail usually won't line-break if there's no punctuation to break at.
// - Doubleclicking selects the whole number as one word if it's all
// alphanumeric.
//
#ifndef BITCOIN_BASE58_H
#define BITCOIN_BASE58_H

#include <algorithm>
#include <limits>
#include <string.h>
#include <string>
#include <vector>

#include "fc/logging/logging.h"
#include <fc/types/bignum.h>
#include <openssl/bn.h>
#include <stdexcept>
#include <vector>

static const char *pszBase58 =
    "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

// Encode a byte sequence as a base58-encoded string
inline std::string EncodeBase58(const unsigned char *pbegin,
                                const unsigned char *pend) {
  CAutoBN_CTX pctx;
  CBigNum bn58 = 58;
  CBigNum bn0 = 0;

  // Convert big endian data to little endian
  // Extra zero at the end make sure bignum will interpret as a positive number
  std::vector<unsigned char> vchTmp(pend - pbegin + 1, 0);
  reverse_copy(pbegin, pend, vchTmp.begin());

  // Convert little endian data to bignum
  CBigNum bn;
  bn.setvch(vchTmp);

  // Convert bignum to std::string
  std::string str;
  // Expected size increase from base58 conversion is approximately 137%
  // use 138% to be safe
  str.reserve((pend - pbegin) * 138 / 100 + 1);
  CBigNum dv;
  CBigNum rem;
  while (bn > bn0) {
    if (!BN_div(dv.to_bignum(), rem.to_bignum(), bn.to_bignum(),
                bn58.to_bignum(), pctx))
      throw bignum_error("EncodeBase58 : BN_div failed");
    bn = dv;
    unsigned int c = rem.getulong();
    str += pszBase58[c];
  }

  // Leading zeroes encoded as base58 zeros
  for (const unsigned char *p = pbegin; p < pend && *p == 0; p++)
    str += pszBase58[0];

  // Convert little endian std::string to big endian
  reverse(str.begin(), str.end());
  //    slog( "Encode '%s'", str.c_str() );
  return str;
}

// Encode a byte vector as a base58-encoded string
inline std::string EncodeBase58(const std::vector<unsigned char> &vch) {
  return EncodeBase58(&vch[0], &vch[0] + vch.size());
}

// Decode a base58-encoded string psz into byte vector vchRet
// returns true if decoding is succesful
inline bool DecodeBase58(const char *psz, std::vector<unsigned char> &vchRet) {
  CAutoBN_CTX pctx;
  vchRet.clear();
  CBigNum bn58 = 58;
  CBigNum bn = 0;
  CBigNum bnChar;
  while (isspace(*psz))
    psz++;

  // Convert big endian string to bignum
  for (const char *p = psz; *p; p++) {
    const char *p1 = strchr(pszBase58, *p);
    if (p1 == NULL) {
      while (isspace(*p))
        p++;
      if (*p != '\0') {
        // slog( "%s  '%c'", pszBase58,*p );
        return false;
      }
      break;
    }
    bnChar.setulong(p1 - pszBase58);
    if (!BN_mul(bn.to_bignum(), bn.to_bignum(), bn58.to_bignum(), pctx))
      throw bignum_error("DecodeBase58 : BN_mul failed");
    bn += bnChar;
  }

  // Get bignum as little endian data
  std::vector<unsigned char> vchTmp = bn.getvch();

  // Trim off sign byte if present
  if (vchTmp.size() >= 2 && vchTmp.end()[-1] == 0 && vchTmp.end()[-2] >= 0x80)
    vchTmp.erase(vchTmp.end() - 1);

  // Restore leading zeros
  int nLeadingZeros = 0;
  for (const char *p = psz; *p == pszBase58[0]; p++)
    nLeadingZeros++;
  vchRet.assign(nLeadingZeros + vchTmp.size(), 0);

  // Convert little endian data to big endian
  reverse_copy(vchTmp.begin(), vchTmp.end(), vchRet.end() - vchTmp.size());
  return true;
}

// Decode a base58-encoded string str into byte vector vchRet
// returns true if decoding is succesful
inline bool DecodeBase58(const std::string &str,
                         std::vector<unsigned char> &vchRet) {
  return DecodeBase58(str.c_str(), vchRet);
}

namespace fc {

std::string to_base58(const char *d, size_t s) {
  return EncodeBase58((const unsigned char *)d, (const unsigned char *)d + s)
      .c_str();
}

std::string to_base58(const std::vector<char> &d) {
  if (d.size())
    return to_base58(d.data(), d.size());
  return std::string();
}
std::vector<char> from_base58(const std::string &base58_str) {
  std::vector<unsigned char> out;
  if (!DecodeBase58(base58_str.c_str(), out)) {
    LOG_FATAL("Unable to decode base58 string %s", base58_str.c_str());
  }
  return std::vector<char>((const char *)out.data(),
                           ((const char *)out.data()) + out.size());
}
/**
 *  @return the number of bytes decoded
 */
size_t from_base58(const std::string &base58_str, char *out_data,
                   size_t out_data_len) {
  // slog( "%s", base58_str.c_str() );
  std::vector<unsigned char> out;
  if (!DecodeBase58(base58_str.c_str(), out)) {
    LOG_FATAL("Unable to decode base58 string %s", base58_str.c_str());
  }
  fc_assert(out.size() <= out_data_len, "out.size() > out_data_len");
  memcpy(out_data, out.data(), out.size());
  return out.size();
}
} // namespace fc

#endif
