#ifndef TYPES_H
#define TYPES_H
#include <stdio.h>
#include <stdint.h>
#define MAX_FILENAME 32
#define MAGIC_NUMBER 0x0B00B1E5
#define ERROR_MAGIC_NUMBER 0xDEAD2BAD
#define SUPERBLOCK_SIZE 32
#define FILE_ENTRY_SIZE MAX_FILENAME + 16

#pragma pack(1)
typedef struct SuperBlock {
    uint32_t magic_number;
    uint32_t max_files;
    uint32_t num_files;
    uint32_t num_blocks;
    uint32_t block_size;
    uint32_t bit_map_start;
    uint32_t file_table_start;
    uint32_t data_start;
} SuperBlock;

#pragma pack(1)
typedef struct BitMap {
    uint32_t size;
    uint8_t map[];
} BitMap;
#pragma pack(1)
typedef struct FileEntry {
    char name[MAX_FILENAME];
    uint32_t size;
    uint32_t start_block;
    uint32_t num_blocks;
    uint32_t last_byte; // To keep track of unused bytes in last used block when internal fragmentation happens
} FileEntry;

typedef struct FileTable {
    uint32_t size;
    FileEntry table[];
} FileTable;

#pragma pack(1)
typedef struct MetaData {
    SuperBlock super_block;
    BitMap *bit_map;
    FileTable *table;
} MetaData;

// SuperBlock readSuperBlock(FILE *fp);
//
// int writeSuperBlock(const SuperBlock *super_block, FILE *fp);
//
// BitMap *readBitMap(FILE *fp);
//
// int writeBitMap(const BitMap *map, FILE *fp);
//
// FileTable *readFileTable(FILE *fp);
//
// int writeFileTable(const FileTable *table, FILE *fp);
//
// FileEntry readFileEntry(FILE *fp);
//
// int writeFileEntry(const FileEntry *entry, FILE *fp);
//
// MetaData readMetadata(FILE *fp);
//
// int writeMetaData(const MetaData *header, FILE *fp);

MetaData readMetaData(FILE *fp);

int writeMetaData(const MetaData *header, FILE *fp);



// MetaData createMetaData(uint32_t max_file, uint32_t num_blocks, uint32_t block_size);

#endif //TYPES_H
