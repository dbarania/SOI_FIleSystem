#ifndef UTILS_H
#define UTILS_H
#include <inttypes.h>
int ends_with(const char *str, const char *suffix);
uint32_t to_big_endian(const uint32_t val);

#endif //UTILS_H
