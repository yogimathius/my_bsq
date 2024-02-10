#include <my_bsq.h>

int my_bsq(char *file) {
    char *buffer;
    ssize_t bytes_read;
    int result = read_file(file, &buffer, &bytes_read);
    if (result){
        return result;
    }
    write(STDOUT_FILENO, buffer, bytes_read);
    printf("\n");
    char **matrix;
    int rows, cols;
    result = parse_buffer_to_matrix(buffer, bytes_read, &matrix, &rows, &cols);
    if (result){
        printf("Failed to parse buffer to matrix.\n");
        free(buffer);
        return result;
    }
    int **results = malloc(rows * sizeof(int *));
    for (int i = 0; i < rows; i++) {
        results[i] = malloc(cols * sizeof(int));
    }
    check_matrix(matrix, rows, cols, results);
    for (int i = 0; i < rows; i++) {
        free(results[i]);
    }
    free(results);
    free_matrix(matrix, rows);
    free(buffer);
    return 0;
}

int main(int ac , char **av){
    if (ac == 2){
        return my_bsq(av[1]);
    }
    printf(HELP);
    return EXIT_SUCCESS;
}