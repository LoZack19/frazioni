CC = gcc
CFLAGS = -Wall -I./
LDFLAGS = -lm

SRC_DIR = src
INCLUDE_DIR = include

SRCS := $(wildcard $(SRC_DIR)/*.c) main.c
OBJS := $(patsubst %.c,%.o,$(SRCS))

EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(EXECUTABLE) $(OBJS)

.PHONY: all clean
