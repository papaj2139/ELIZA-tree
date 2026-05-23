CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -g
LIB_SRC = src/tree.c src/eliza.c
LIB_OBJ = $(LIB_SRC:.c=.o)

.PHONY: all clean train chat

all: eliza-train eliza-chat

eliza-train: src/train_main.o $(LIB_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

eliza-chat: src/chat_main.o $(LIB_OBJ)
	$(CC) $(CFLAGS) -o $@ $^ -lm

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f src/*.o eliza-train eliza-chat

train: eliza-train
	./eliza-train

chat: eliza-chat
	./eliza-chat
