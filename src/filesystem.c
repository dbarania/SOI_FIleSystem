#include "../include/filesystem.h"
#include "../include/types.h"

struct {
    FILE *file;
    MetaData data;
} filesystem;
