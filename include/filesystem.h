#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <stdbool.h>
#include <stdint.h>
#include "types.h"

#define BLOCK_USED   "■"
#define BLOCK_FREE   "▢"

int create_filesystem(const char* filename, int n_files, int n_blocks, int block_size);

int save_filesystem_meta();

int copy_to_filesystem(const char* file_name);

int copy_from_filesystem(const char* file_name, const char* destination);

int list_filesystem();

int remove_from_filesystem(const char* file_name);

int delete_filesystem(const char *filesystem_name);

int format_filesystem(bool deep);

int defragment_filesystem();

int diagnostics_of_filesystem();

int load_filesystem(const char* filesystem_name);

// int check_block(const uint32_t block_index);
//
// void set_block(const uint32_t block_index);
//
// void clear_block(const uint32_t block_index);
//
// int find_free_blocks(const uint32_t num_blocks);
//
// int add_file_entry(const FileEntry* entry);
//
// int save_filesystem_meta();
#endif //FILESYSTEM_H
