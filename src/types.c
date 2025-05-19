#include "../include/types.h"

#include <stdlib.h>
#include "../include/utils.h"

SuperBlock readSuperBlock(FILE *fp) {
    SuperBlock block = {0};

    if (fp == NULL) {
        block.MagicNumber = ERROR_MAGIC_NUMBER;
        return block;
    }

    uint32_t buf[8];
    size_t items_read = fread(buf, sizeof(uint32_t), 8, fp);

    if (items_read != 8) {
        block.MagicNumber = ERROR_MAGIC_NUMBER;
        return block;
    }

    block.MagicNumber = to_big_endian(buf[0]);

    if (block.MagicNumber != MAGIC_NUMBER) {
        block.MagicNumber = ERROR_MAGIC_NUMBER;
        return block;
    }

    block.max_files = buf[1];
    block.num_files = buf[2];
    block.num_blocks = buf[3];
    block.block_size = buf[4];
    block.bit_map_start = buf[5];
    block.file_table_start = buf[6];
    block.data_start = buf[7];

    return block;
}

int writeSuperBlock(const SuperBlock *super_block, FILE *fp) {
    if (super_block == NULL || fp == NULL) {
        return -1; // Invalid input
    }
    uint32_t magic_number_inverted = to_big_endian(super_block->MagicNumber);
    // printf("%d", magic_number_inverted);
    if (fwrite(&magic_number_inverted, sizeof(uint32_t), 1, fp) != 1) return -2;
    if (fwrite(&super_block->max_files, sizeof(uint32_t), 1, fp) != 1) return -2;
    if (fwrite(&super_block->num_files, sizeof(uint32_t), 1, fp) != 1) return -2;
    if (fwrite(&super_block->num_blocks, sizeof(uint32_t), 1, fp) != 1) return -2;
    if (fwrite(&super_block->block_size, sizeof(uint32_t), 1, fp) != 1) return -2;
    if (fwrite(&super_block->bit_map_start, sizeof(uint32_t), 1, fp) != 1) return -2;
    if (fwrite(&super_block->file_table_start, sizeof(uint32_t), 1, fp) != 1) return -2;
    if (fwrite(&super_block->data_start, sizeof(uint32_t), 1, fp) != 1) return -2;
    return 1;
}

BitMap *readBitMap(FILE *fp) {
    if (fp == NULL) {
        return NULL;
    }
    uint32_t size;
    if (fread(&size, sizeof(uint32_t), 1, fp) != 1) {
        return NULL;
    }
    BitMap *bitmap = malloc(sizeof(BitMap) + size * sizeof(uint8_t));
    if (bitmap == NULL) {
        return NULL;
    }

    bitmap->size = size;

    if (fread(bitmap->map, sizeof(uint8_t), size, fp) != size) {
        free(bitmap);
        return NULL;
    }
    return bitmap;
}

int writeBitMap(const BitMap *map, FILE *fp) {
    if (map == NULL || fp == NULL) {
        return -1;
    }
    if (fwrite(&map->size, sizeof(uint32_t), 1, fp) != 1) {
        return -2;
    }
    if (fwrite(map->map, sizeof(uint8_t), map->size / 8, fp) != map->size) {
        return -3;
    }
    return 0;
}

FileTable *readFileTable(FILE *fp) {
    if (!fp) return NULL;

    size_t size;
    if (fread(&size, sizeof(size_t), 1, fp) != 1) return NULL;

    FileTable *table = malloc(sizeof(FileTable) + size * sizeof(FileEntry));
    if (!table) return NULL;

    table->size = size;
    for (size_t i = 0; i < size; i++) {
        table->table[i] = readFileEntry(fp);
    }

    return table;
}

int writeFileTable(const FileTable *table, FILE *fp) {
    if (!table || !fp) return -1;
    if (fwrite(&table->size, sizeof(uint32_t), 1, fp) != 1) return -2;
    for (size_t i = 0; i < table->size; i++) {
        if (writeFileEntry(&table->table[i], fp) != 0) return -3;
    }

    return 0;
}


FileEntry readFileEntry(FILE *fp) {
    FileEntry entry = {0};

    if (!fp) return entry;

    fread(entry.name, sizeof(char), MAX_FILENAME, fp);
    fread(&entry.size, sizeof(uint32_t), 1, fp);
    fread(&entry.start_block, sizeof(uint32_t), 1, fp);
    fread(&entry.num_blocks, sizeof(uint32_t), 1, fp);
    fread(&entry.last_byte, sizeof(uint32_t), 1, fp);

    return entry;
}


int writeFileEntry(const FileEntry *entry, FILE *fp) {
    if (!entry || !fp) return -1;

    if (fwrite(entry->name, sizeof(char), MAX_FILENAME, fp) != MAX_FILENAME) return -2;
    if (fwrite(&entry->size, sizeof(uint32_t), 1, fp) != 1) return -2;
    if (fwrite(&entry->start_block, sizeof(uint32_t), 1, fp) != 1) return -2;
    if (fwrite(&entry->num_blocks, sizeof(uint32_t), 1, fp) != 1) return -2;
    if (fwrite(&entry->last_byte, sizeof(uint32_t), 1, fp) != 1) return -2;

    return 0;
}


MetaData readMetadata(FILE *fp) {
    MetaData meta = {0};
    if (!fp) return meta;
    fread(&meta.super_block, sizeof(SuperBlock), 1, fp);
    meta.bit_map = readBitMap(fp);
    if (!meta.bit_map) {
        fprintf(stderr, "Failed to read BitMap\n");
        return meta;
    }
    meta.table = readFileTable(fp);
    if (!meta.table) {
        fprintf(stderr, "Failed to read FileTable\n");
        free(meta.bit_map);
        meta.bit_map = NULL;
    }
    return meta;
}


int writeMetaData(const MetaData *header, FILE *fp) {
    if (!header || !fp) return -1;
    writeSuperBlock(&header->super_block, fp);
    writeBitMap(header->bit_map, fp);
    writeFileTable(header->table, fp);
    const int32_t current_pos = ftell(fp);
    int32_t bytes_left = header->super_block.data_start - current_pos;
    printf("data start %d\n", header->super_block.data_start);
    printf("%d", bytes_left);
    int32_t chunk = 512;
    uint8_t zeroes[512] = {0};
    while (bytes_left > 0) {
        if (bytes_left < chunk) {
            chunk = bytes_left;
        }
        if (fwrite(zeroes, 1, chunk, fp) != chunk) {
            return -1;
        }

        bytes_left -= chunk;
    }
    // if (fwrite(&header->super_block, sizeof(SuperBlock), 1, fp) != 1) return -2;
    // if (writeBitMap(header->bit_map, fp) != 0) return -3;
    // if (writeFileTable(header->table, fp) != 0) return -4;

    return 0;
}
