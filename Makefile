STD = -std=c++17
INCLUDES = -I./includes
CFLAGS = -Wall -Wextra -Wshadow -Wformat-nonliteral -Wformat-security -Wtype-limits -O2

co2-logger: objects/main.o objects/rs232.o
	g++ ${STD} ${INCLUDES} -o $@ $^

objects/main.o: source/main.cpp
	g++ ${STD} ${INCLUDES} -c -o $@ $^

objects/rs232.o: source/rs232.c
	gcc ${INCLUDES} ${CFLAGS} -c -o $@ $^
