#ifndef __MY_BSQ_
#define __MY_BSQ_

#include <stdio.h>  /* printf(3) */
#include <stdlib.h> /* EXIT_SUCCESS */
#include <string.h>
#include <fcntl.h>  /* open(2) */
#include <unistd.h>
#include <sys/stat.h>

#define SIZE 16
#define __MatrixClass_self_alignement 0xC0

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
  int (*const read)(const char *filename);
  int (*const get_size)();
  void (*const alloc)();
  void (*const build)();
  void (*const debug)();
  void (*const print)();
  void (*const free)();
  void (*const bsq)();
};

extern const struct MatrixClass {
  struct Matrix (*new)();
} Matrix;

#endif
