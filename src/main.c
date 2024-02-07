#include <my_bsq.h>

int main(int ac , char **av){
    if (ac == 2){
        printf("Received map: %s\n", av[1]);
    }
    else {
        printf(HELP);
    }
    return EXIT_SUCCESS;
}