CFLAGS = -g -Wall -std=c++14
CC = g++

main: shiro.cpp
	$(CC) $(CFLAGS) $< -o shiro

clean:
	$(RM) shiro