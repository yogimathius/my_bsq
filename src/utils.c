#include <my_bsq.h>

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

void build_matrix(char *buffer, char ***matrix, int rows, int cols, ssize_t bytes_read){
    int k = 0;
    int i = 0;
    int j;

    while (i < rows){
        j = 0;
        while (j < cols) {
            if (k < bytes_read) {
                if (buffer[k] != '\n') {
                    (*matrix)[i][j] = buffer[k];
                }
                else {
                    j -= 1;
                }
                k += 1;
            }
            else {
                (*matrix)[i][j] = ' ';
            }
            j += 1;
        }
        i += 1;
    }
}

void print_matrix(char **matrix, int rows, int cols) {
    int i = 0;
    int j = 0;

    while (i < rows){
        while (j < cols){
            printf("%c", matrix[i][j]);
            j += 1;
        }
        printf("\n");
        j = 0;
        i += 1;
    }
}

void free_matrix(char **matrix, int rows){
    int i = 0;
    while (i < rows){
        if (matrix[i]){
            free(matrix[i]);
        }
        i += 1;
    }
}

int parse_buffer_to_matrix(char *buffer, ssize_t bytes_read, char ***matrix, int *rows, int *cols) {
    int row_count = 1; // We start with 1 as at this point at least one row is guaranteed from bytes read
    int col_count = 0;
    int max_col_count = 0;
    int new_line = 0;
    int i = 0;

    while (i < bytes_read){
        if (buffer[i] == '\n'){
            if (new_line == 0){
                row_count += 1;
                if (col_count > max_col_count){
                    max_col_count = col_count;
                }
                col_count = 0;
            }
            new_line = 1;
        }
        else {
            new_line = 0;
            col_count += 1;
        }
        i += 1;
    }
    i = 0;
    *rows = row_count;
    *cols = max_col_count;
    printf("Rows: %d\nCols: %d\nMax cols: %d\n", *rows, *cols, max_col_count);
    if ((*matrix = malloc(sizeof(char*) * row_count)) == NULL) {
        perror("Error allocating memory for matrix\n");
        return 1;
    }
    while (i < row_count){
        if (((*matrix)[i] = malloc(max_col_count)) == NULL) {
            perror("Error allocating memory for matrix row\n");
            return 1;
        }
        i += 1;
    }
    build_matrix(buffer, matrix, row_count, max_col_count, bytes_read);
    print_matrix(*matrix, row_count, max_col_count);
    free_matrix(*matrix, row_count);
    return 0;
}