#include <my_bsq.h>

typedef struct matrix {
    int **matrix;
    int rows;
    int cols;
} matrix_t;

int read_file(char *file, char **buffer, ssize_t *bytes_read){
    struct stat s;
    int fd = open(file, O_RDONLY);
    if (fd == -1){
        perror("Error opening the file");
        return 1;
    }
    if (fstat(fd, &s) == -1){
        perror("Error getting file size");
        close(fd);
        return 1;
    }
    if ((*buffer = malloc(s.st_size)) == NULL){
        perror("Error allocating memory for buffer");
        close(fd);
        return 1;
    }
    if ((*bytes_read = read(fd, *buffer, s.st_size)) == -1) {
        printf("Failed to read file: '%s'\n", file);
        free(*buffer);
        close(fd);
        return 1;
    }
    printf("Read %zd bytes from file: '%s'\n", *bytes_read, file);

    close(fd);
    return 0;
}

int get_rows_and_cols(char *buffer, matrix_t *matrix, ssize_t *bytes_read) {
    const char *newline_ptr = strchr(buffer, '\n');
    if (newline_ptr == NULL) {
        printf("No newline found in buffer\n");
        return 1;
    }

    char size_str[16];
    strncpy(size_str, buffer, newline_ptr - buffer);
    size_str[newline_ptr - buffer] = '\0';
    printf("Size string: %s\n", size_str);
    matrix->rows = atoi(size_str);
    matrix->cols = matrix->rows;

    int newline_index = (int)(newline_ptr - buffer);

    memmove(buffer, buffer + newline_index + 1, *bytes_read - newline_index - 1);
    *bytes_read -= newline_index + 1;
    printf("Rows: %d, Cols: %d\n", matrix->rows, matrix->cols);
    return 0;
}

void allocate_matrix(matrix_t *matrix){
    if ((matrix->matrix = malloc(sizeof(char*) * matrix->rows)) == NULL) {
        perror("Error allocating memory for matrix\n");
        return;
    }
    int i = 0;
    while (i < matrix->rows){
        if (((matrix->matrix)[i] = malloc(matrix->cols * sizeof(int))) == NULL) {
            perror("Error allocating memory for matrix row\n");
            return;
        }
        i += 1;
    }
}

void build_largest_square(int **matrix, int maxRow, int maxCol, int maxValue) {
    int i = maxRow;
    int j = maxCol;
    while (i > maxRow - maxValue){
        while (j > maxCol - maxValue){
            if (matrix[i][j] == 0) {
                printf("WRONG CELL: %d, %d\n", i, j);
            }
            matrix[i][j] = 'x';
            j -= 1;
        }
        j = maxCol;
        i -= 1;
    }
}

#define min(a, b) ((a < b) ? a : b)
void build_matrix(const char *buffer, matrix_t *matrix, ssize_t bytes_read){
    int k = 0;
    int i = 0;

    int maxValue = 0;
    int maxRow = 0;
    int maxCol = 0;

    while (i < matrix->rows){
        int j = 0;
        while (j < matrix->cols) {
            if (i == 0 || j == 0) {
                matrix->matrix[i][j] = (buffer[k] == 'o') ? 0 : 1;
            } else {
                if (buffer[k] == '.') {
                    matrix->matrix[i][j] = min(min(matrix->matrix[i-1][j], matrix->matrix[i][j-1]), matrix->matrix[i-1][j-1]) + 1;
                    if (matrix->matrix[i][j] > maxValue) {
                        maxValue = matrix->matrix[i][j];
                        maxRow = i;
                        maxCol = j;
                    }
                } else {
                    matrix->matrix[i][j] = 0;
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

    printf("Max value: %d, Max row: %d, Max col: %d\n", maxValue, maxRow, maxCol);
    // build_largest_square(matrix->matrix, maxRow, maxCol, maxValue);
}

void print_matrix(matrix_t *matrix) {
    int i = 0;
    int j = 0;

    while (i < matrix->rows){
        while (j < matrix->cols){
            printf("%d", matrix->matrix[i][j]);
            j += 1;
        }
        printf("\n");
        j = 0;
        i += 1;
    }
}

void free_matrix(matrix_t *matrix){
    int i = 0;
    while (i < matrix->rows){
        if (matrix->matrix[i]){
            free(matrix->matrix[i]);
        }
        i += 1;
    }
}

int parse_buffer_to_matrix(char *buffer, ssize_t bytes_read) {
    matrix_t *matrix = malloc(sizeof(matrix_t));
    matrix->rows = 10;
    matrix->cols = 10;

    int response = get_rows_and_cols(buffer, matrix, &bytes_read);
    if (response == 1) {
        free(matrix);
        return 1;
    }

    allocate_matrix(matrix);

    build_matrix(buffer, matrix, bytes_read);

    print_matrix(matrix);

    free_matrix(matrix);

    return 0;
}


//  Old build_matrix function that uses a char matrix instead of an int matrix
// void build_matrix(char *buffer, char ***matrix, int rows, int cols, ssize_t bytes_read){
//     int k = 0;
//     int i = 0;
//     int j;

//     while (i < rows){
//         j = 0;
//         while (j < cols) {
//             if (k < bytes_read) {
//                 if (buffer[k] != '\n') {
//                     (*matrix)[i][j] = buffer[k];
//                 }
//                 else {
//                     j -= 1;
//                 }
//                 k += 1;
//             }
//             else {
//                 (*matrix)[i][j] = ' ';
//             }
//             j += 1;
//         }
//         i += 1;
//     }
// }