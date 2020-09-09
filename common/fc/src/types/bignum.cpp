//
// Created by 李卫东 on 2019-10-22.
//

#include <fc/types/bignum.h>

const CBigNum operator+(const CBigNum &a, const CBigNum &b) {
  CBigNum r;
  if (!BN_add(r.to_bignum(), a.to_bignum(), b.to_bignum()))
    throw bignum_error("CBigNum::operator+ : BN_add failed");
  return r;
}

const CBigNum operator-(const CBigNum &a, const CBigNum &b) {
  CBigNum r;
  if (!BN_sub(r.to_bignum(), a.to_bignum(), b.to_bignum()))
    throw bignum_error("CBigNum::operator- : BN_sub failed");
  return r;
}

const CBigNum operator-(const CBigNum &a) {
  CBigNum r(a);
  BN_set_negative(r.to_bignum(), !BN_is_negative(r.to_bignum()));
  return r;
}

const CBigNum operator*(const CBigNum &a, const CBigNum &b) {
  CAutoBN_CTX pctx;
  CBigNum r;
  if (!BN_mul(r.to_bignum(), a.to_bignum(), b.to_bignum(), pctx))
    throw bignum_error("CBigNum::operator* : BN_mul failed");
  return r;
}

const CBigNum operator/(const CBigNum &a, const CBigNum &b) {
  CAutoBN_CTX pctx;
  CBigNum r;
  if (!BN_div(r.to_bignum(), NULL, a.to_bignum(), b.to_bignum(), pctx))
    throw bignum_error("CBigNum::operator/ : BN_div failed");
  return r;
}

const CBigNum operator%(const CBigNum &a, const CBigNum &b) {
  CAutoBN_CTX pctx;
  CBigNum r;
  if (!BN_mod(r.to_bignum(), a.to_bignum(), b.to_bignum(), pctx))
    throw bignum_error("CBigNum::operator% : BN_div failed");
  return r;
}

const CBigNum operator<<(const CBigNum &a, unsigned int shift) {
  CBigNum r;
  if (!BN_lshift(r.to_bignum(), a.to_bignum(), shift))
    throw bignum_error("CBigNum:operator<< : BN_lshift failed");
  return r;
}

const CBigNum operator>>(const CBigNum &a, unsigned int shift) {
  CBigNum r = a;
  r >>= shift;
  return r;
}

bool operator==(const CBigNum &a, const CBigNum &b) {
  return (BN_cmp(a.to_bignum(), b.to_bignum()) == 0);
}
bool operator!=(const CBigNum &a, const CBigNum &b) {
  return (BN_cmp(a.to_bignum(), b.to_bignum()) != 0);
}
bool operator<=(const CBigNum &a, const CBigNum &b) {
  return (BN_cmp(a.to_bignum(), b.to_bignum()) <= 0);
}
bool operator>=(const CBigNum &a, const CBigNum &b) {
  return (BN_cmp(a.to_bignum(), b.to_bignum()) >= 0);
}
bool operator<(const CBigNum &a, const CBigNum &b) {
  return (BN_cmp(a.to_bignum(), b.to_bignum()) < 0);
}
bool operator>(const CBigNum &a, const CBigNum &b) {
  return (BN_cmp(a.to_bignum(), b.to_bignum()) > 0);
}