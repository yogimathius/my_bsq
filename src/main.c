#include <my_bsq.h>

int my_bsq(const char *file){
    char *buffer;
    ssize_t bytes_read;
    int result = read_file(file, &buffer, &bytes_read);
    if (result){
        perror("Error reading file");
        return result;
    }
    result = parse_buffer_to_matrix(buffer, bytes_read);
    if (result){
        printf("Failed to parse buffer to matrix.\n");
    }
    return result;
}

int main(const int ac , const char **av){
    if (ac == 2){
        return my_bsq(av[1]);
    }
    printf(HELP);
    return EXIT_SUCCESS;
}