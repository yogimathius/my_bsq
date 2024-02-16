#ifndef __MY_BSQ_
#define __MY_BSQ_

#include <stdio.h>  /* printf(3) */
#include <stdlib.h> /* EXIT_SUCCESS */
#include <string.h> 
#include <fcntl.h>  /* open(2) */
#include <unistd.h>
#include <sys/stat.h>

#define SIZE 16

#define HELP "\
my_bsq -- finds and prints the biggest square in a map\n\
my_bsq [file.txt] ...\n"

struct Matrix {
  ssize_t size;
  char *buffer;
  int **matrix;
  int rows;
  int cols;
  int max_val;
  int max_row;
  int max_col;
  int (*const read)(const char *filename, struct Matrix* self);
  int (*const get_size)(struct Matrix* self);
  void (*const alloc)(struct Matrix* self);
  void (*const build)(const struct Matrix* self);
  void (*const debug)(const struct Matrix* self);
  void (*const print)(const struct Matrix* self);
  void (*const free)(struct Matrix* self);
  void (*const bsq)(struct Matrix* self);/* fill_largest_square */
};

extern const struct MatrixClass {
  struct Matrix (*new)();
} /*matrixProxy*/ Matrix;

#endif
