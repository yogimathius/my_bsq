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

void build_rows_and_cols(char *buffer, int *rows, int *cols, ssize_t bytes_read){
    int i = 0;
    int new_line = 0;
    int col_count = 0;
    int row_count = 1;

    while (i < bytes_read){
        if (buffer[i] == '\n'){
            if (new_line == 0){
                row_count += 1;
                if (col_count > *cols){
                    *cols = col_count;
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
    *rows = row_count;
    *cols = col_count;
}




int parse_buffer_to_matrix(char *buffer, ssize_t bytes_read, char ***matrix, int *rows, int *cols) {
    build_rows_and_cols(buffer, rows, cols, bytes_read);
    printf("Rows: %d\nCols: %d\n", *rows, *cols);
    if ((*matrix = malloc(sizeof(char*) * *rows)) == NULL) {
        perror("Error allocating memory for matrix\n");
        return 1;
    }
    int i = 0;
    while (i < *rows){
        if (((*matrix)[i] = malloc(*cols)) == NULL) {
            perror("Error allocating memory for matrix row\n");
            return 1;
        }
        i += 1;
    }

    build_matrix(buffer, matrix, *rows, *cols, bytes_read);
    print_matrix(*matrix, *rows, *cols);
    return 0;
}

void check_matrix(char **matrix, int rows, int cols, int **result){
    for (int col = 0; col < cols; col++) {
      result[0][col] = (matrix[0][col] == 'o') ? 1 : 0;
      result[col][0] = (matrix[col][0] == 'o') ? 1 : 0;
    }

    for (int i = 1; i < rows; i++) {
        for (int j = 1; j < cols; j++) {
            if (matrix[i][j] == 'o') {
                int above = result[i - 1][j];
                int left = result[i][j - 1];
                int left_diagonal = result[i - 1][j - 1];
                result[i][j] = above + left + left_diagonal + 1;
            } else {
                result[i][j] = 0;
            }
        }
    }

    for (int k = 0; k < rows; k++) {
        for (int l = 0; l < cols; l++) {
        printf("%d ", result[k][l]);
        }
        printf("\n");
    }

}