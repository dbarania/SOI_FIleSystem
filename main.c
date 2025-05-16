#include <stdio.h>
#include <string.h>
#include "include/commands.h"
#include "include/utils.h"

#define SUFFIX ".img"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <subcommand> [args...]\n", argv[0]);
        return 17;
    }

    const char *subcommand = argv[1];

    argc -= 1;
    argv += 1;

    const char *file_name = argv[1];
    argc -= 1;
    argv += 1;

    if (!ends_with(file_name,SUFFIX)) {
        return -1;
    }
    if (strcmp(subcommand, "create") == 0) {
        return command_create(file_name, argc, argv);
    } else if (strcmp(subcommand, "cp_to") == 0) {
        return command_cp_to(file_name, argc, argv);
    } else if (strcmp(subcommand, "cp_from") == 0) {
        return command_cp_from(file_name, argc, argv);
    } else if (strcmp(subcommand, "list") == 0) {
        return command_list(file_name, argc, argv);
    } else if (strcmp(subcommand, "remove") == 0) {
        return command_remove(file_name, argc, argv);
    } else if (strcmp(subcommand, "delete") == 0) {
        return command_delete(file_name, argc, argv);
    } else if (strcmp(subcommand, "format") == 0) {
        return command_format(file_name, argc, argv);
    } else if (strcmp(subcommand, "diagnostics") == 0) {
        return command_diagnostics(file_name, argc, argv);
    } else if (strcmp(subcommand, "defragment") == 0) {
        return command_defragment(file_name, argc, argv);
    } else {
        return -1;
    }
}
