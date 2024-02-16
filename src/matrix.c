#include <my_bsq.h>

static int
get_size(char *buffer, struct Matrix* self, ssize_t *bytes_read) {
    const char *newline_ptr = strchr(buffer, '\n');
    if (newline_ptr == NULL) {
        printf("No newline found in buffer\n");
        return 1;
    }       

    char size_str[16];
    strncpy(size_str, buffer, newline_ptr - buffer);
    size_str[newline_ptr - buffer] = '\0';

    self->rows = atoi(size_str);
    self->cols = self->rows;

    int newline_index = (int)(newline_ptr - buffer);

    memmove(buffer, buffer + newline_index + 1, *bytes_read - newline_index - 1);
    *bytes_read -= newline_index + 1;
    return 0;
}

static void
allocate(struct Matrix* self){
    if ((self->matrix = malloc(sizeof(char*) * self->rows)) == NULL) {
        perror("Error allocating memory for matrix\n");
        return;
    }
    int i = 0;
    while (i < self->rows){
        if (((self->matrix)[i] = malloc(self->cols * sizeof(int))) == NULL) {
            perror("Error allocating memory for matrix row\n");
            return;
        }
        i += 1;
    }
}

static void
build(const char *buffer, struct Matrix* self, ssize_t bytes_read){
    allocate(self);

    int k = 0;
    int i = 0;

    self->max_value = 0;
    self->max_row = 0;
    self->max_col = 0;

    while (i < self->rows){
        int j = 0;
        while (j < self->cols) {
            if (i == 0 || j == 0) {
                self->matrix[i][j] = (buffer[k] == 'o') ? 0 : 1;
            } else {
                if (buffer[k] == '.') {
                    self->matrix[i][j] = min(min(self->matrix[i-1][j], self->matrix[i][j-1]), self->matrix[i-1][j-1]) + 1;
                    if (self->matrix[i][j] > self->max_value) {
                        self->max_value = self->matrix[i][j];
                        self->max_row = i;
                        self->max_col = j;
                    }
                } else {
                    self->matrix[i][j] = 0;
                }
            } 
            if (k < bytes_read) {
                if (buffer[k] == '\n') {
                    j -= 1;
                }
                k += 1;
            }
            j += 1;
        }
        i += 1;
    }

}

static void
print(const struct Matrix* self) {
    int i = 0;
    int j = 0;

    while (i < self->rows){
        while (j < self->cols){
            printf("%d", self->matrix[i][j]);
            j += 1;
        }
        printf("\n");
        j = 0;
        i += 1;
    }
}

static void 
free_matrix(struct Matrix* self){
    int i = 0;
    while (i < self->rows){
        if (self->matrix[i]){
            free(self->matrix[i]);
        }
        i += 1;
    }
}

static void
fill_largest_square(char *buffer, const struct Matrix* self) {
    int i = 0;
    int k = 0;

    
    int row_lower_bound = self->max_row - self->max_value + 1;
    int col_lower_bound = self->max_col - self->max_value + 1;

    while (i < self->rows){
        int j = 0;
        while (j < self->cols){
            if (buffer[k] == '\n') {
                j -= 1;
            } else {
                if (buffer[k] == '.' && i >= row_lower_bound && i <=  self->max_row && j >= col_lower_bound && j <= self->max_col) {
                    buffer[k] = 'x';
                }
            }
            j += 1;
            k += 1;
        }
        i += 1;
    }
}

static struct Matrix
new()
{
  return (struct Matrix) {
    .matrix = NULL,
    .rows = 0,
    .cols = 0,
    .max_value = 0,
    .max_row = 0,
    .max_col = 0,
    .get_size = (void*) &get_size,
    .allocate = (void*) &allocate,
    .build = (void*) &build,
    .print = (void*) &print,
    .free_matrix = (void*) &free_matrix,
    .fill_largest_square = (void*) &fill_largest_square
  };
}

const struct MatrixClass /*matrixProxy*/ Matrix = {
  .new = &new,
};

int parse_buffer(char *buffer, ssize_t bytes_read) {
    struct Matrix matrix = Matrix.new();
    
    matrix.get_size(buffer, &matrix, &bytes_read);
    matrix.build(buffer, &matrix, bytes_read);
    matrix.fill_largest_square(buffer, &matrix);
    print_buffer(buffer, bytes_read);
    matrix.free_matrix(&matrix);
    return 0;
}