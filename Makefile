HEADERS = src/
SRC = src/*.c
LIBRARY = -lncurses
FLAGS = -Wall -Wextra -W -g
FILE = pong

default:
	gcc $(FLAGS) $(SRC) $(LIBRARY) -I $(HEADERS) -o $(FILE)

run:
	./$(FILE)
