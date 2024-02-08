#ifndef __MY_BSQ_
#define __MY_BSQ_

#include <stdio.h> /* printf(3) */
#include <stdlib.h> /* EXIT_SUCCESS */

#define HELP "\
my_bsq -- finds and prints the biggest square in a map\n\
my_bsq [file.txt] ...\n"

int read_file(char *filename, char **buffer, ssize_t *bytes_read);

int parse_buffer_to_matrix(char *buffer, ssize_t bytes_read, char ***matrix, int *rows, int *cols);

#endif
