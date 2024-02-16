#include <my_bsq.h>

int read_file(char *file, char **buffer, ssize_t *bytes_read){
    struct stat s;
    int fd = open(file, O_RDONLY);
    if (fd == -1){
        perror("Error opening the file");
        return 1;
    }
    if (fstat(fd, &s) == -1){
        perror("Error getting file size");
        close(fd);
        return 1;
    }
    if ((*buffer = malloc(s.st_size)) == NULL){
        perror("Error allocating memory for buffer");
        close(fd);
        return 1;
    }
    if ((*bytes_read = read(fd, *buffer, s.st_size)) == -1) {
        printf("Failed to read file: '%s'\n", file);
        free(*buffer);
        close(fd);
        return 1;
    }
    close(fd);
    return 0;
}

void print_buffer(const char *buffer, ssize_t bytes_read) {
    int i = 0;
    while (i < bytes_read) {
        if (buffer[i] == '\n') {
            printf("\n");
        } else {
            printf("%c", buffer[i]);
        }
        i += 1;
    }
}
