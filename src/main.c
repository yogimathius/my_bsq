#include <my_bsq.h>

int my_bsq(const char *file){
    struct Matrix matrix = Matrix.new();

    int result = matrix.read(file, &matrix);
    if (result){
        perror("Error reading file");
        return result;
    }
    matrix.bsq(&matrix);
    matrix.free(&matrix);
    return 0;
}

int main(const int ac , const char **av){
    if (ac == 2){
        return my_bsq(av[1]);
    }
    printf(HELP);
    return EXIT_SUCCESS;
}