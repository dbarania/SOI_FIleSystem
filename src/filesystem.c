#include "filesystem.h"
#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// private functions structs

struct {
    FILE *file;
    MetaData data;
} filesystem;

int find_free_blocks(const uint32_t num_blocks);

void clear_block(const uint32_t block_index);

void set_block(const uint32_t block_index);

int check_block(const uint32_t block_index);

int add_file_entry(const FileEntry *entry);

// Public functions

int create_filesystem(const char *filename, int n_files, int n_blocks, int block_size) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Failed to open file");
        return -1;
    }

    const int bit_map_bytes = (n_blocks + 7) / 8;
    const int file_table_bytes = n_files * FILE_ENTRY_SIZE;


    MetaData meta;

    meta.super_block.magic_number = MAGIC_NUMBER;
    meta.super_block.max_files = n_files;
    meta.super_block.num_files = 0;
    meta.super_block.num_blocks = n_blocks;
    meta.super_block.block_size = block_size;
    meta.super_block.bit_map_start = SUPERBLOCK_SIZE;
    meta.super_block.file_table_start = meta.super_block.bit_map_start + 4 + bit_map_bytes;
    meta.super_block.data_start = meta.super_block.file_table_start + file_table_bytes;

    meta.bit_map = malloc(sizeof(BitMap) + bit_map_bytes);
    if (!meta.bit_map) {
        fclose(fp);
        return -2;
    }

    meta.bit_map->size = bit_map_bytes * 8;
    memset(meta.bit_map->map, 0, bit_map_bytes); // mark all blocks as free

    meta.table = malloc(sizeof(FileTable) + n_files * sizeof(FileEntry));
    if (!meta.table) {
        free(meta.bit_map);
        fclose(fp);
        return -3;
    }
    meta.table->size = n_files; // no files yet

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

    for (int i = 0; i < n_blocks; ++i) {
        fwrite(zero_block, block_size, 1, fp);
    }

    free(zero_block);
    free(meta.bit_map);
    free(meta.table);
    fclose(fp);

    return 0;
}

int save_filesystem_meta() {
    fseek(filesystem.file, 0, SEEK_SET);
    writeMetaData(&filesystem.data, filesystem.file);
    return 1;
}

int copy_to_filesystem(const char *file_name) {
    FILE *file = fopen(file_name, "rb");
    if (!file) {
        perror("fopen");
        return -1;
    }
    const uint32_t block_size = filesystem.data.super_block.block_size;
    fseek(file, 0,SEEK_END);
    const uint32_t size = ftell(file);
    fseek(file, 0,SEEK_SET);

    const uint32_t last_byte = size % block_size;
    const uint32_t num_blocks = (last_byte == 0) ? size / block_size : size / block_size + 1;
    const int start_block = find_free_blocks(num_blocks);

    if (start_block == -1) {
        printf("No space for this file, reconsider defragmentation");
        return -1;
    }
    if (strlen(file_name) >= MAX_FILENAME) {
        printf("Name of the file is too long");

        return -1;
    }

    FileEntry entry;
    strcpy(entry.name, file_name);
    entry.size = size;
    entry.start_block = start_block;
    entry.num_blocks = num_blocks;
    entry.last_byte = last_byte;

    if (add_file_entry(&entry) == -1) {
        return -1;
    }
    const uint32_t start_byte = filesystem.data.super_block.data_start + start_block * block_size;
    fseek(filesystem.file, start_byte,SEEK_SET);
    char *buffer = malloc(block_size);
    if (!buffer) {
        perror("malloc failed");
        fclose(file);
        return -1;
    }
    uint32_t total_written = 0;
    while (total_written < size) {
        const uint32_t to_read = (size - total_written > block_size) ? block_size : size - total_written;
        fread(buffer, to_read - 1, 1, file);
        fwrite(buffer, 1, to_read, filesystem.file);
        total_written += to_read;
    }
    free(buffer);
    fclose(file);
    return 1;
}

int copy_from_filesystem(const char *file_name, const char *destination) { return 1; }

int list_filesystem() { return 1; }

int remove_from_filesystem(const char *file_name) { return 1; }

int delete_filesystem() { return 1; }

int format_filesystem() { return 1; }

int defragment_filesystem() { return 1; }

int diagnostics_of_filesystem() { return 1; }

int load_filesystem(const char *filesystem_name) {
    filesystem.file = fopen(filesystem_name, "rb+");
    filesystem.data = readMetaData(filesystem.file);
    if (filesystem.data.super_block.magic_number != MAGIC_NUMBER) {
        return -1;
    }
    return 1;
}

// Private functions definition

int check_block(const uint32_t block_index) {
    const uint32_t byte_index = block_index / 8;
    const uint8_t bit_index = block_index % 8;
    return (filesystem.data.bit_map->map[byte_index] & (1 << bit_index));
}

void set_block(const uint32_t block_index) {
    const uint32_t byte_index = block_index / 8;
    const uint8_t bit_index = block_index % 8;
    filesystem.data.bit_map->map[byte_index] |= (1 << bit_index);
}

void clear_block(const uint32_t block_index) {
    const uint32_t byte_index = block_index / 8;
    const uint8_t bit_index = block_index % 8;
    filesystem.data.bit_map->map[byte_index] &= ~(1 << bit_index);
}


int find_free_blocks(const uint32_t num_blocks) {
    uint32_t free_count = 0;
    int first_block = 0;
    for (int block_index = 0; block_index < filesystem.data.bit_map->size; ++block_index) {
        if (check_block(block_index) == 0) {
            if (free_count == 0) {
                first_block = block_index;
            }
            free_count++;
            if (free_count >= num_blocks) {
                return first_block;
            }
        } else {
            free_count = 0;
        }
    }
    return -1;
}

int add_file_entry(const FileEntry *entry) {
    if (filesystem.data.super_block.num_files >= filesystem.data.super_block.max_files) {
        return -1;
    }
    const uint32_t size = filesystem.data.super_block.num_files;
    memcpy(&filesystem.data.table->table[size], entry, FILE_ENTRY_SIZE);
    // filesystem.data.table->size++;
    for (uint32_t block_id = 0; block_id < entry->num_blocks; ++block_id) {
        set_block(entry->start_block + block_id);
    }
    filesystem.data.super_block.num_files++;
    return 1;
}
