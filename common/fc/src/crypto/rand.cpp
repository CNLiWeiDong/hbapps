#include "fc/logging/logging.h"
#include <openssl/err.h>
#include <openssl/rand.h>
#include <string>

namespace fc {

// void RAND_seed(const void *buf,int num);
//随机数生成前设定种子；
void rand_seed(const void *buf, int num) { RAND_seed(buf, num); }
void init_seed() {
  static bool init_status = false;
  if (init_status)
    return;
  auto t = 1;
  uint64_t seed_data = (uint64_t)(&t);
  rand_seed(&seed_data, sizeof(seed_data));
}
// int  RAND_bytes(unsigned char *buf,int num);
//根据加密算法生成随机数，其实也是一个伪随机数，但是，如果在调用此函数之前，设定好随机种子，那么生成的随机数是不能被预先计算出来的。
void rand_bytes(char *buf, int count) {
  init_seed();
  int result = RAND_bytes((unsigned char *)buf, count);
  if (result != 1)
    LOG_FATAL("Error calling OpenSSL's RAND_bytes(): %d",
              (uint32_t)ERR_get_error());
}

void rand_pseudo_bytes(char *buf, int count) {
  init_seed();
  // RAND_pseudo_bytes is deprecated in favor of RAND_bytes as of OpenSSL 1.1.0
#if OPENSSL_VERSION_NUMBER < 0x10100000L
  int result = RAND_pseudo_bytes((unsigned char *)buf, count);
  if (result == -1)
    LOG_FATAL("Error calling OpenSSL's RAND_pseudo_bytes(): %d",
              (uint32_t)ERR_get_error());
#else
  rand_bytes(buf, count);
#endif
}

} // namespace fc
