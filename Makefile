CC = g++
CFLAGS = -Wall -g -O2 -std=c++17

SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

run: cfgparser
	./cfgparser

cfgparser: $(OBJS)
	$(CC) $(CFLAGS) -o cfgparser $(OBJS)

clean:
	rm -f cfgparser $(OBJS)

.PHONY: run clean
