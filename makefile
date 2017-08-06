CFLAGS = -g -Wall -std=c++14
CC = g++

main: src/shiro.cpp
	mkdir -p build
	$(CC) $(CFLAGS) $< -o build/shiro

clean:
	$(RM) shiro