#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <my_bsq.h>

int read_file(char *filename, char **buffer, ssize_t *bytes_read) {
    int fd;
    struct stat file_stat;

    fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening the file");
        return 1;
    }

    if (fstat(fd, &file_stat) == -1) {
        perror("Error getting file size");
        close(fd);
        return 1;
    }

    *buffer = (char *)malloc(file_stat.st_size);
    if (*buffer == NULL) {
        perror("Error allocating memory for buffer");
        close(fd);
        return 1;
    }

    *bytes_read = read(fd, *buffer, file_stat.st_size);
    if (*bytes_read == -1) {
        perror("Error reading from the file");
        free(*buffer);
        close(fd);
        return 1;
    }

    printf("Bytes read: %zd\n", *bytes_read);
    close(fd);

    return 0;
}

void build_matrix(char *buffer, char ***matrix, int rows, int cols, ssize_t bytes_read) {
    int i, j;
    int k = 0;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            if (k < bytes_read) {
                if (buffer[k] != '\n') {
                    (*matrix)[i][j] = buffer[k];
                } else {
                    j--;
                }
                k++;
            }
            else {
                (*matrix)[i][j] = ' ';
            }
        }
    }
}


void print_matrix(char **matrix, int rows, int cols) {
    int i, j;

    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%c", matrix[i][j]);
        }
        printf("\n");
    }
}

int parse_buffer_to_matrix(char *buffer, ssize_t bytes_read, char ***matrix, int *rows, int *cols) {
    int i;
    int row_count = 1; // We start with 1 as at this point at least one row is guaranteed from bytes read
    int col_count = 0;
    int max_col_count = 0;
    int new_line = 0;

    for (i = 0; i < bytes_read; i++) {
        if (buffer[i] == '\n') {
            if (new_line == 0) {
                row_count++;
                if (col_count > max_col_count) {
                    max_col_count = col_count;
                }
                col_count = 0;
            }
            new_line = 1;
        } else {
            new_line = 0;
            col_count++;
        }
    }

    *rows = row_count;
    *cols = max_col_count;

    printf("Rows: %d\n", *rows);
    printf("Cols: %d\n", *cols);
    printf("Max cols: %d\n", max_col_count);
    *matrix = (char **)malloc(sizeof(char *) * row_count);
    if (*matrix == NULL) {
        perror("Error allocating memory for matrix");
        return 1;
    }

    for (i = 0; i < row_count; i++) {
        (*matrix)[i] = (char *)malloc(sizeof(char) * max_col_count);
        if ((*matrix)[i] == NULL) {
            perror("Error allocating memory for matrix row");
            return 1;
        }
    }

    build_matrix(buffer, matrix, row_count, max_col_count, bytes_read);
    
    print_matrix(*matrix, row_count, max_col_count);

    return 0;
}