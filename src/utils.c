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
    write(STDOUT_FILENO, buffer, bytes_read);
    printf("\n");

    close(fd);
    return 0;
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

void allocate_matrix(int ***matrix, int rows, int cols){
    if ((*matrix = malloc(sizeof(char*) * rows)) == NULL) {
        perror("Error allocating memory for matrix\n");
        return;
    }
    int i = 0;
    while (i < rows){
        if (((*matrix)[i] = malloc(cols)) == NULL) {
            perror("Error allocating memory for matrix row\n");
            return;
        }
        i += 1;
    }
}

void build_matrix(const char *buffer, int rows, int cols, ssize_t bytes_read){
    int k = 0;
    int i = 0;
    int **results = malloc(rows * sizeof(int *));
    for (int p = 0; p < rows; p++) {
        results[p] = malloc(cols * sizeof(int));
    }
    while (i < rows){
        int j = 0;
        while (j < cols) {
            if (i == 0) {
                results[i][j] = (buffer[k] == 'o') ? 1 : 0;
            } else if (j == 0) {
                results[i][j] = (buffer[k] == 'o') ? 1 : 0;
            } else {
                if (buffer[k] == 'o') {
                    int above = results[i - 1][j];
                    int left = results[i][j - 1];
                    int left_diagonal = results[i - 1][j - 1];
                    results[i][j] = above + left + left_diagonal + 1;
                } else {
                    results[i][j] = 0;
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

    for (int m = 0; m < rows; m++) {
        for (int n = 0; n < cols; n++) {
            printf("%d ", results[m][n]);
        }
        printf("\n");
    }
}

void print_matrix(int **matrix, int rows, int cols) {
    int i = 0;
    int j = 0;

    while (i < rows){
        while (j < cols){
            printf("%d", matrix[i][j]);
            j += 1;
        }
        printf("\n");
        j = 0;
        i += 1;
    }
}

void free_matrix(int **matrix, int rows){
    int i = 0;
    while (i < rows){
        if (matrix[i]){
            free(matrix[i]);
        }
        i += 1;
    }
}

int parse_buffer_to_matrix(char *buffer, ssize_t bytes_read, int ***matrix, int *rows, int *cols) {
    build_rows_and_cols(buffer, rows, cols, bytes_read);

    allocate_matrix(matrix, *rows, *cols);

    build_matrix(buffer, *rows, *cols, bytes_read);
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