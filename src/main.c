#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <my_bsq.h>

int main(int ac , char **av){
    int fd;
    char *buffer;
    ssize_t bytes_read;
    struct stat file_stat;

    if (ac == 2){
        printf("Received map: %s\n", av[1]);
    }
    else {
        printf(HELP);
    }

    fd = open(av[1], O_RDONLY);
    if (fd == -1) {
        perror("Error opening the file");
        return 1;
    }

    if (fstat(fd, &file_stat) == -1) {
        perror("Error getting file size");
        close(fd);
        return 1;
    }

    buffer = (char *)malloc(file_stat.st_size);
    if (buffer == NULL) {
        perror("Error allocating memory for buffer");
        close(fd);
        return 1;
    }

    bytes_read = read(fd, buffer, file_stat.st_size);
    if (bytes_read == -1) {
        perror("Error reading from the file");
        free(buffer);
        close(fd);
        return 1;
    }

    printf("Bytes read: %zd\n", bytes_read);
    printf("Content read from the file:\n");
    write(STDOUT_FILENO, buffer, bytes_read);

    if (close(fd) == -1) {
        perror("Error closing the file");
        free(buffer);
        return 1;
    }

    free(buffer);

    return EXIT_SUCCESS;
}