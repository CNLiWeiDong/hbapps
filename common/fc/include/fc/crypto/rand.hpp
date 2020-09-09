#pragma once

namespace fc {
void init_seed();
void rand_seed(const void *buf, int num);
/* provides access to the OpenSSL random number generator */
void rand_bytes(char *buf, int count);
void rand_pseudo_bytes(char *buf, int count);
} // namespace fc
