CFLAGS = -g -Wall
CC = g++

main: shiro.cpp
	$(CC) $(CFLAGS) $< -o shiro

clean:
	$(RM) shiro