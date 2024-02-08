#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <my_bsq.h>

int my_bsq(char *filename) {
    char *buffer;
    ssize_t bytes_read;

    int result = read_file(filename, &buffer, &bytes_read);
    if (result != 0) {
        printf("Failed to read file.\n");
        return 1;
    }

    printf("Content read from the file:\n");
    write(STDOUT_FILENO, buffer, bytes_read);

    free(buffer);

    return 0;
}


int main(int ac , char **av){

    if (ac == 2){
        printf("Received map: %s\n", av[1]);
    }
    else {
        printf(HELP);
    }

    my_bsq(av[1]);

    return EXIT_SUCCESS;
}