CC=gcc
CFLAGS=-Wall -Wextra -Werror
BIN=my_bsq
SRC=src/main.c src/utils.c
INC=inc

all:
	$(CC) $(CFLAGS) -I$(INC) $(SRC) -o $(BIN)
clean:
	rm -f $(OBJ)
fclean: clean
	rm -f $(BIN)
re: fclean all
