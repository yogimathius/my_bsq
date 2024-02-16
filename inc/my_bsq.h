#ifndef __MY_BSQ_
#define __MY_BSQ_

#include <stdio.h>  /* printf(3) */
#include <stdlib.h> /* EXIT_SUCCESS */
#include <string.h> 
#include <fcntl.h>  /* open(2) */
#include <unistd.h>
#include <sys/stat.h>

#define SIZE 16
#define min(a, b) ((a < b) ? a : b)
// ^ macros with logic strictly forbidden

#define HELP "\
my_bsq -- finds and prints the biggest square in a map\n\
my_bsq [file.txt] ...\n"

int read_file(char *filename, char **buffer, ssize_t *bytes_read);
int parse_buffer(char *buffer, ssize_t bytes_read);
void print_buffer(const char *buffer, ssize_t bytes_read);

struct Matrix {
    int **matrix;
    int rows;
    int cols;
    int max_value;
    int max_row;
    int max_col;
    int (* const get_size)(char *buffer, struct Matrix* self, ssize_t *bytes_read);
    void (* const allocate)(struct Matrix* self);
    void (* const build)(const char *buffer, struct Matrix* self, ssize_t bytes_read);
    void (* const print)(struct Matrix* self);
    void (* const free_matrix)(struct Matrix* self);
    void (* const fill_largest_square)(char *buffer, struct Matrix* self);
};

extern const struct MatrixClass {
  struct Matrix (*new)();
} /*matrixProxy*/ Matrix;

#endif
