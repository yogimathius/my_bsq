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

typedef struct matrix {
    int **matrix;
    int rows;
    int cols;
    int max_val;
    int max_row;
    int max_col;
    int row_lower_bound;
    int row_upper_bound;
    int col_lower_bound;
    int col_upper_bound;
} matrix_t;

int read_file(const char *filename, char **buffer, ssize_t *bytes_read);
int parse_buffer_to_matrix(char *buffer, ssize_t bytes_read);

#endif
