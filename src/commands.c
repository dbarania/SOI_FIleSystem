#include "../include/commands.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "../include/filesystem.h"

int command_create(const char *file_name, int argc, char **argv) {
    static struct option create_options[] = {
        {STR_NFILES, required_argument, 0, 0},
        {STR_NBLOCKS, required_argument, 0, 0},
        {STR_BLOCKSIZE, required_argument, 0, 0},
        {STR_HELP, no_argument, 0, 'h'}
    };
    int option_index = 0;
    int c;
    int n_files = 16;
    int n_blocks = 256;
    int block_size = 1024;
    while ((c = getopt_long(argc, argv, "h", create_options, &option_index)) != -1) {
        if (c == 0) {
            const char *name = create_options[option_index].name;
            if (strcmp(name, STR_NFILES) == 0) {
                n_files = atoi(optarg);
            } else if (strcmp(name, STR_NBLOCKS) == 0) {
                n_blocks = atoi(optarg);
            } else if (strcmp(name,STR_BLOCKSIZE) == 0) {
                block_size = atoi(optarg);
            }
        } else if (c == 'h') {
            printf("Not written yet, you are trying to create a virtual filesystem");
            return 0;
        }
    }

    create_filesystem(file_name, n_files, n_blocks, block_size);
    return 1;
}

int command_cp_to(const char *file_name, int argc, char **argv) {
    static struct option cp_to_options[] = {
        {STR_FILE, required_argument, 0, 'f'}
    };
    int c;
    int option_index = 0;
    const char *file_path = "";
    while ((c = getopt_long(argc, argv, "f", cp_to_options, &option_index)) != -1) {
        if (c == 0) {
            const char *name = cp_to_options[option_index].name;
            if (strcmp(name, STR_FILE) == 0) {
                file_path = optarg;
            }
        } else if (c == 'f') {
            file_path = optarg;
        }
    }
    if (strcmp(file_path, "") == 0) {
        printf("You need to give a file to copy");
    } else {
        load_filesystem(file_name);
        copy_to_filesystem(file_path);
    }

    return 1;
}

int command_cp_from(const char *file_name, int argc, char **argv) {
    static struct option cp_from_options[] = {
        {STR_FILE, required_argument, 0, 'f'},
        {STR_DESTINATION, required_argument, 0, 'd'}
    };

    int c;
    int option_index = 0;
    const char *file_path = "";
    const char *destination_path = "";
    while ((c = getopt_long(argc, argv, "f:d", cp_from_options, &option_index)) != -1) {
        if (c == 0) {
            const char *name = cp_from_options[option_index].name;
            if (strcmp(name, STR_FILE) == 0) {
                file_path = optarg;
            } else if (strcmp(name, STR_DESTINATION) == 0) {
                destination_path = optarg;
            }
        } else if (c == 'f') {
            file_path = optarg;
        } else if (c == 'd') {
            destination_path = optarg;
        }
    }

    if (strcmp(file_path, "") == 0) {
        printf("You need to give a file to copy");
        return -1;
    } else if (strcmp(destination_path, "") == 0) {
        printf("You need to provide a destination to copy to");
        return -1;
    } else {
        load_filesystem(file_name);
        copy_from_filesystem(file_path, destination_path);
    }
    return 1;
}

int command_list(const char *file_name, int argc, char **argv) {
    static struct option list_options[] = {
        {STR_HELP, no_argument, 0, 'h'}
    };
    int c;
    int option_index = 0;
    while ((c = getopt_long(argc, argv, "f:d", list_options, &option_index)) != -1) {
        if (c == 0) {
            const char *name = list_options[option_index].name;
            if (strcmp(name, STR_HELP) == 0) {
                printf("Not written yet, you are trying to create a virtual filesystem");
                return 0;
            }
        } else if (c == 'h') {
            printf("Not written yet, you are trying to create a virtual filesystem");
            return 0;
        }
    }
    load_filesystem(file_name);
    list_filesystem();
    return 1;
}

int command_remove(const char *file_name, int argc, char **argv) {
    static struct option remove_options[] = {
        {STR_FILE, required_argument, 0, 'f'}
    };
    return 1;
}

int command_delete(const char *file_name, int argc, char **argv) {
    return delete_filesystem(file_name);
}

int command_format(const char *file_name, int argc, char **argv) {
    static struct option format_options[] = {
        {STR_QUICK,no_argument, 0, 'q'},
        {STR_DEEP,no_argument, 0, 0}
    };
    return 1;
}

int command_diagnostics(const char *file_name, int argc, char **argv) {
    static struct option diagnostics_options[] = {
        {STR_VERBOSE, required_argument, 0, 'v'}
    };
    return 1;
}

int command_defragment(const char *file_name, int argc, char **argv) {
    return 1;
}
