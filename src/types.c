#include "types.h"

#include <stdlib.h>
#include "utils.h"
// private functions


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

FileTable *readFileTable(FILE *fp) {
    if (!fp) return NULL;
    uint32_t size;
    if (fread(&size, sizeof(uint32_t), 1, fp) != 1) return NULL;


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


// Public functions

MetaData readMetaData(FILE *fp) {
    MetaData meta = {0};
    if (!fp) {
        goto error;
    }
    fseek(fp, 0, SEEK_SET);
    // Reading super block
    size_t items_read = fread(&meta.super_block, sizeof(SuperBlock), 1, fp);
    if (items_read != 1) {
        goto error;
    }

    meta.super_block.magic_number = to_big_endian(meta.super_block.magic_number);

    if (meta.super_block.magic_number != MAGIC_NUMBER) {
        goto error;
    }

    // Reading bitmap
    meta.bit_map = malloc(sizeof(BitMap) + meta.super_block.num_blocks / 8);
    if (!meta.bit_map) goto error;
    items_read = fread(&meta.bit_map->size, sizeof(uint32_t), 1, fp);
    if (items_read != 1) {
        goto error;
    }
    items_read = fread(&meta.bit_map->map, sizeof(uint8_t), meta.bit_map->size / 8, fp);

    if (items_read != meta.bit_map->size / 8 || meta.bit_map->size != meta.super_block.num_blocks) {
        goto error;
    }

    // Reading FileTable
    meta.table = readFileTable(fp);

    // meta.table = malloc(sizeof(FileTable) + meta.super_block.max_files * sizeof(FileEntry));
    // if (!meta.table) goto error;
    //
    // items_read = fread(&meta.table->size, sizeof(uint32_t), 1, fp);
    // if (items_read != 1) {
    //     goto error;
    // }
    // items_read = fread(meta.table->table, sizeof(FileEntry), meta.super_block.max_files, fp);
    //
    // if (items_read != meta.table->size || meta.table->size != meta.super_block.max_files) {
    //     goto error;
    // }
    return meta;

error:
    meta.super_block.magic_number = ERROR_MAGIC_NUMBER;
    free(meta.bit_map);
    free(meta.table);
    return meta;
}

int writeMetaData(const MetaData *header, FILE *fp) {
    if (!header || !fp) return -1;
    // Write super block

    uint32_t magic_number_inverted = to_big_endian(header->super_block.magic_number);
    size_t items_wrote = fwrite(&magic_number_inverted, sizeof(uint32_t), 1, fp);
    if (items_wrote != 1) {
        return -1;
    }

    items_wrote = fwrite(&header->super_block.max_files, sizeof(uint32_t), 7, fp);
    if (items_wrote != 7) {
        return -1;
    }
    // Write bitmap
    items_wrote = fwrite(&header->bit_map->size, sizeof(uint32_t), 1, fp);
    if (items_wrote != 1) {
        return -1;
    }
    items_wrote = fwrite(header->bit_map->map, sizeof(uint8_t), header->bit_map->size / 8, fp);
    if (items_wrote != header->bit_map->size / 8) {
        return -1;
    }
    // Write file table
    // items_wrote = fwrite(&header->table->size, sizeof(uint32_t), 1, fp);
    // if (items_wrote != 1) {
    //     return -1;
    // }

    writeFileTable(header->table, fp);
    return 0;
}