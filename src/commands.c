#include "../include/commands.h"

#include <stdlib.h>

int command_create(const char *file_name, int argc, char **argv) {
    static struct option create_options[]{
        {STR_NFILES, required_argument, 0, 0},
        {STR_NBLOCKS, required_argument, 0, 0},
        {STR_BLOCKSIZE, required_argument, 0, 0},
        {STR_HELP, no_argument, 0, 'h'}
    };
    int option_index = 0;
    int c;
    int n_files;
    int n_blocks;
    int block_size;
    while ((c = getopt_long(argc, argv, "h", create_options, &option_index)) != -1) {
        if (c == 0) {
            const char *name = create_options[option_index].name;
            if (strcmp(name, STR_NFILES) == 0) {
                n_files = atoi(optarg);
            }
            else if (strcmp(name, STR_NBLOCKS) == 0) {
                n_blocks = atoi(optarg);
            }
            else if (strcmp(name,STR_BLOCKSIZE) == 0) {
                block_size = atoi(optarg);
            }
        }
        else if (c == 'h') {
            printf("Not written yet, you are trying to create a virtual filesystem");
            return 0;
        }
        else {
            return -1;
        }
    }
    return 1;
}

int command_cp_to(const char *file_name, int argc, char **argv) {
    static struct option cp_to_options[]{
        {STR_FILE, required_argument, 0, 'f'}
    };
    return 1;
}

int command_cp_from(const char *file_name, int argc, char **argv) {
    static struct option cp_from_options[]{
        {STR_FILE, required_argument, 0, 'f'},
        {STR_DESTINATION, required_argument, 0, 'd'}
    };
    return 1;
}

int command_list(const char *file_name, int argc, char **argv) {
    return 1;
}

int command_remove(const char *file_name, int argc, char **argv) {
    static struct option remove_options[]{
        {STR_FILE, required_argument, 0, 'f'}
    };
    return 1;
}

int command_delete(const char *file_name, int argc, char **argv) {
    return 1;
}

int command_format(const char *file_name, int argc, char **argv) {
    static struct option format_options[]{
        {STR_QUICK,no_argument, 0, 'q'},
        {STR_DEEP,no_argument, 0, 0}
    };
    return 1;
}

int command_diagnostics(const char *file_name, int argc, char **argv) {
    static struct option diagnostics_options[]{
        {STR_VERBOSE, required_argument, 0, 'v'}
    };
    return 1;
}

int command_defragment(const char *file_name, int argc, char **argv) {
    return 1;
}
