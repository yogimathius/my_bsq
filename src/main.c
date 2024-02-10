#include <my_bsq.h>

int my_bsq(char *file) {
    char *buffer;
    ssize_t bytes_read;
    int result = read_file(file, &buffer, &bytes_read);
    if (result){
        return result;
    }
    
    int **matrix;
    int rows, cols;
    result = parse_buffer_to_matrix(buffer, bytes_read, &matrix, &rows, &cols);
    if (result){
        printf("Failed to parse buffer to matrix.\n");
        free(buffer);
        return result;
    }
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