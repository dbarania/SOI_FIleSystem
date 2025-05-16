#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdio.h>
#include  <unistd.h>
#include <inttypes.h>
#include <getopt.h>
#include <string.h>


#define STR_NFILES "n_files"
#define STR_NBLOCKS "n_blocks"
#define STR_BLOCKSIZE "block_size"
#define STR_HELP "help"
#define STR_FILE "file"
#define STR_DESTINATION "destination"
#define STR_QUICK "quick"
#define STR_DEEP "deep"
#define STR_VERBOSE "verbose"

int command_create(const char * file_name, int argc, char **argv);

int command_cp_to(const char * file_name, int argc, char **argv);

int command_cp_from(const char * file_name, int argc, char **argv);

int command_list(const char * file_name, int argc, char **argv);

int command_remove(const char * file_name, int argc, char **argv);

int command_delete(const char * file_name, int argc, char **argv);

int command_format(const char * file_name, int argc, char **argv);

int command_diagnostics(const char * file_name, int argc, char **argv);

int command_defragment(const char * file_name, int argc, char **argv);

#endif //COMMANDS_H
