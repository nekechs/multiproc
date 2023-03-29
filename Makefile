CC = gcc
CFLAGS = -std=c99 -Wall -g -fsanitize=address,undefined

debug:
	CPPFLAGS += -DDEBUG=1