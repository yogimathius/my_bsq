#include <my_bsq.h>

int my_bsq(const char *file){
    char *buffer;
    ssize_t bytes_read;
    int result = read_file(file, &buffer, &bytes_read);
    if (result){
        perror("Error reading file");
        return result;
    }
    write(STDOUT_FILENO, buffer, bytes_read);
    printf("\n");

    struct Matrix matrix = Matrix.new();
    
    matrix.get_size(buffer, &matrix, &bytes_read);
    matrix.build(buffer, &matrix, bytes_read);
    matrix.fill_largest_square(buffer, &matrix);
    print_buffer(buffer, bytes_read);
    matrix.free_matrix(&matrix);

    free(buffer);
    return 0;
}

int main(const int ac , const char **av){
    if (ac == 2){
        return my_bsq(av[1]);
    }
    printf(HELP);
    return EXIT_SUCCESS;
}