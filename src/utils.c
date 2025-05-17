#include "../include/utils.h"
#include <string.h>

int ends_with(const char *str, const char *suffix) {
    if (!str || !suffix)
        return 0;
    const size_t len_str = strlen(str);
    const size_t len_suffix = strlen(suffix);
    if (len_suffix >  len_str)
        return 0;
    return strncmp(str + len_str - len_suffix, suffix, len_suffix) == 0;
}