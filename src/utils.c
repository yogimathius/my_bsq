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