#include <my_bsq.h>

void debug(const matrix_t *m){
    dprintf(1,
    "Rows: %d, Cols: %d\n"
    "Max value: %d\n"
    "Max row: %d\n" 
    "Max col: %d\n"
    "Row lower bound: %d\n"
    "Row upper bound: %d\n"
    "Col lower bound: %d\n"
    "Col upper bound: %d\n",
    m->rows, m->cols, m->max_val, m->max_row, m->max_col,
    m->row_lower_bound, m->row_upper_bound, m->col_lower_bound, m->col_upper_bound);
}

void print(const matrix_t *m) {
    int i = 0;
    int j = 0;

    while (i < m->rows){
        while (j < m->cols){
            printf("%i", m->matrix[i][j]);
            j += 1;
        }
        printf("\n");
        j = 0;
        i += 1;
    }
}

int read_file(const char *file, char **buffer, ssize_t *bytes_read){
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

int get_rows_and_cols(char *buffer, matrix_t *m, ssize_t *bytes_read) {
    const char *LF = strchr(buffer, '\n');
    if (LF == NULL) {
        printf("No newline found in buffer\n");
        return 1;
    }
    char size_str[SIZE];
    strncpy(size_str, buffer, LF - buffer);
    size_str[LF - buffer] = '\0';
    m->cols = m->rows = atoi(size_str);
    int newline_index = (int)(LF - buffer);
    memmove(buffer, buffer + newline_index + 1, *bytes_read - newline_index - 1);
    *bytes_read -= newline_index + 1;
    return 0;
}

matrix_t *alloc(char *buffer, ssize_t bytes_read){
    matrix_t *m = malloc(sizeof(matrix_t));
    if (m){
        m->cols = m->rows = 0;
        int r = get_rows_and_cols(buffer, m, &bytes_read);
        if (r == 1) {
            free(m);
            return NULL;
        }
        if ((m->matrix = malloc(sizeof(char*) * m->rows)) == NULL) {
            perror("Error allocating memory for matrix\n");
            return NULL;
        }
        r = 0;
        while (r < m->rows){
            if (((m->matrix)[r] = malloc(m->cols * sizeof(int))) == NULL) {
                perror("Error allocating memory for matrix row\n");
                return NULL;
            }
            r += 1;
        }
    }
    return m;
}

matrix_t *build(char *buffer, ssize_t bytes_read){
    matrix_t *m = alloc(buffer, bytes_read);
    if (m == NULL){
        return NULL;
    }
    m->max_val = 0;
    m->max_row = 0;
    m->max_col = 0;
    int k = 0;
    int i = 0;
    while (i < m->rows){
        int j = 0;
        while (j < m->cols){
            if (i == 0 || j == 0){
                m->matrix[i][j] = (buffer[k] == 'o') ? 0 : 1;
            }
            else {
                if (buffer[k] == '.') {
                    m->matrix[i][j] = min(min(m->matrix[i-1][j], m->matrix[i][j-1]), m->matrix[i-1][j-1]) + 1;
                    if (m->matrix[i][j] > m->max_val) {
                        m->max_val = m->matrix[i][j];
                        m->max_row = i;
                        m->max_col = j;
                    }
                }
                else {
                    m->matrix[i][j] = 0;
                }
            }
            if (k < bytes_read) {
                if (buffer[k] == '\n'){
                    j -= 1;
                }
                k += 1;
            }
            j += 1;
        }
        i += 1;
    }
    return m;
}

void free_matrix(matrix_t *m){
    while (m->rows){
        if (*m->matrix){
            free(*m->matrix);
        }
        m->matrix += 1;
        m->rows -= 1;
    }
}

void get_boundaries(matrix_t *m){
    if (m->max_val + m->max_row > m->rows) {
        m->row_lower_bound = m->max_row - m->max_val;
        m->row_upper_bound = m->max_row;;
    }
    else {
        m->row_lower_bound = m->max_row;
        m->row_upper_bound = m->max_row + m->max_val;
    }
    if (m->max_val + m->max_col > m->cols) {
        m->col_lower_bound = m->max_col - m->max_val;
        m->col_upper_bound = m->max_col;
    }
    else {
        m->col_lower_bound = m->max_col;
        m->col_upper_bound = m->max_col + m->max_val;
    }
}

void draw(char *buffer, matrix_t *m){
    int i = 0;
    int k = 0;
    get_boundaries(m);
    while (i < m->rows){
        int j = 0;
        while (j < m->cols){
            if (buffer[k] == '\n') {
                j -= 1;
            }
            else {
                if (buffer[k] == '.' && i >= m->row_lower_bound && i <= m->row_upper_bound && j >= m->col_lower_bound && j <= m->col_upper_bound) {
                    buffer[k] = 'x';
                }
            }
            j += 1;
            k += 1;
        }
        i += 1;
    }
}

int parse_buffer_to_matrix(char *buffer, ssize_t bytes_read) {
    matrix_t *m = build(buffer, bytes_read);
    if (m){
        draw(buffer, m);
        dprintf(1, "%.*s", (int)bytes_read, buffer);
        free_matrix(m);
        free(buffer);
        return 0;
    }
    return 1;
}
