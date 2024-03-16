#include <my_bsq.h>

int my_bsq(const char *file){
    struct Matrix matrix = Matrix.new();
    if (matrix.read(file)){
        
        return EXIT_FAILURE;
    }
    matrix.bsq();
    matrix.free();
    return EXIT_SUCCESS;
}

int main(const int ac , const char **av){
    if (ac == 2){
        return my_bsq(av[1]);
    }
    printf(HELP);
    return EXIT_SUCCESS;
}