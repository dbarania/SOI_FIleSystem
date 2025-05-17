#ifndef FILESYSTEM_H
#define FILESYSTEM_H


int create_filesystem(const char *filename, int n_files, int n_blocks, int block_size);



int copy_to_filesystem(const char *file_name);

int copy_from_filesystem(const char *file_name, const char *destination);

int list_filesystem();

int remove_from_filesystem(const char *file_name);

int delete_filesystem();

int format_filesystem();

int defragment_filesystem();

int diagnostics_of_filesystem();

int load_filesystem(const char *filesystem_name);
#endif //FILESYSTEM_H
