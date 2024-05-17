CC = gcc
CFLAGS = -Wall -I.
TARGET = pacman

SRCS = main.c game.c graphics.c sound.c input.c utils.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

rename:
	for file in *.txt; do mv "$$file" "$${file%.txt}"; done

.PHONY: all clean rename
