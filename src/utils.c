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

int build_rows_and_cols(char *buffer, int *rows, int *cols, ssize_t *bytes_read) {
    const char *newline_ptr = strchr(buffer, '\n');
    if (newline_ptr == NULL) {
        printf("No newline found in buffer\n");
        return 1;
    }

    char size_str[16];
    strncpy(size_str, buffer, newline_ptr - buffer);
    size_str[newline_ptr - buffer] = '\0';

    *rows = atoi(size_str);
    *cols = *rows;

    int newline_index = (int)(newline_ptr - buffer);

    memmove(buffer, buffer + newline_index + 1, *bytes_read - newline_index - 1);
    *bytes_read -= newline_index + 1;
    printf("Rows: %d, Cols: %d\n", *rows, *cols);
    return 0;
}

void allocate_matrix(int ***matrix, int rows, int cols){
    if ((*matrix = malloc(sizeof(char*) * rows)) == NULL) {
        perror("Error allocating memory for matrix\n");
        return;
    }
    int i = 0;
    while (i < rows){
        if (((*matrix)[i] = malloc(cols * sizeof(int))) == NULL) {
            perror("Error allocating memory for matrix row\n");
            return;
        }
        i += 1;
    }
}

void build_matrix(const char *buffer, int **result, int rows, int cols, ssize_t bytes_read){
    int k = 0;
    int i = 0;

    while (i < rows){
        int j = 0;
        while (j < cols) {
            if (i == 0 || j == 0) {
                result[i][j] = (buffer[k] == 'o') ? 1 : 0;
            } else {
                if (buffer[k] == 'o') {
                    int above = result[i - 1][j];
                    int left = result[i][j - 1];
                    int left_diagonal = result[i - 1][j - 1];
                    result[i][j] = above + left + left_diagonal + 1;
                } else {
                    result[i][j] = 0;
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
    int response = build_rows_and_cols(buffer, rows, cols, &bytes_read);
    if (response == 1) {
        return 1;
    }

    allocate_matrix(matrix, *rows, *cols);

    build_matrix(buffer, *matrix, *rows, *cols, bytes_read);

    print_matrix(*matrix, *rows, *cols);
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