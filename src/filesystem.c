#include "../include/filesystem.h"
#include "../include/types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct {
    FILE *file;
    MetaData data;
} filesystem;

int create_filesystem(const char *filename, int n_files, int n_blocks, int block_size) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Failed to open file");
        return -1;
    }

    const int bit_map_bytes = (n_blocks + 7) / 8; // 1 bit per block
    const int bit_map_blocks = (bit_map_bytes + block_size - 1) / block_size;

    const int file_table_bytes = n_files * FILE_ENTRY_SIZE;
    const int file_table_blocks = (file_table_bytes + block_size - 1) / block_size;

    const int data_start_block = 1 + bit_map_blocks + file_table_blocks;

    MetaData meta;

    meta.super_block.MagicNumber = MAGIC_NUMBER;
    meta.super_block.max_files = n_files;
    meta.super_block.num_files = 0;
    meta.super_block.num_blocks = n_blocks;
    meta.super_block.block_size = block_size;
    meta.super_block.bit_map_start = 1;
    meta.super_block.file_table_start = 1 + bit_map_blocks;
    meta.super_block.data_start = data_start_block;

    meta.bit_map = malloc(sizeof(BitMap) + bit_map_bytes);
    if (!meta.bit_map) {
        fclose(fp);
        return -2;
    }
    meta.bit_map->size = bit_map_bytes;
    memset(meta.bit_map->map, 0, bit_map_bytes); // mark all blocks as free

    meta.table = malloc(sizeof(FileTable) + n_files * sizeof(FileEntry));
    if (!meta.table) {
        free(meta.bit_map);
        fclose(fp);
        return -3;
    }
    meta.table->size = 0; // no files yet

    if (writeMetaData(&meta, fp) != 0) {
        fprintf(stderr, "Failed to write metadata\n");
        free(meta.bit_map);
        free(meta.table);
        fclose(fp);
        return -4;
    }

    uint8_t *zero_block = calloc(1, block_size);
    if (!zero_block) {
        free(meta.bit_map);
        free(meta.table);
        fclose(fp);
        return -5;
    }

    const int blocks_to_fill = n_blocks - data_start_block;
    for (int i = 0; i < blocks_to_fill; ++i) {
        fwrite(zero_block, block_size, 1, fp);
    }

    free(zero_block);
    free(meta.bit_map);
    free(meta.table);
    fclose(fp);

    return 0;
}


int copy_to_filesystem(const char *file_name) { return 1; }

int copy_from_filesystem(const char *file_name, const char *destination) { return 1; }

int list_filesystem() { return 1; }

int remove_from_filesystem(const char *file_name) { return 1; }

int delete_filesystem() { return 1; }

int format_filesystem() { return 1; }

int defragment_filesystem() { return 1; }

int diagnostics_of_filesystem() { return 1; }

int load_filesystem(const char *filesystem_name) {
    return 1;
}
