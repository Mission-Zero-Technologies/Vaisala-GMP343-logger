STD = -std=c++17
INCLUDES = -I./includes
CFLAGS = -Wall -Wextra -Wshadow -Wformat-nonliteral -Wformat-security -Wtype-limits -O2

co2-logger: objects/main.o objects/rs232.o objects/sensor_interface.o objects/fl2_utils.o objects/flag_node.o objects/flag_list.o objects/flag_manager.o
	g++ ${STD} ${INCLUDES} -o $@ $^

objects/main.o: source/main.cpp
	g++ ${STD} ${INCLUDES} -c -o $@ $^

objects/rs232.o: source/rs232.c
	gcc ${INCLUDES} ${CFLAGS} -c -o $@ $^

objects/sensor_interface.o: source/sensor_interface.cpp
	g++ ${STD} ${INCLUDES} -c -o $@ $^

objects/fl2_utils.o: source/fl2_utils.cpp
	g++ ${STD} ${INCLUDES} -c -o $@ $^

objects/flag_node.o: source/flag_node.cpp
	g++ ${STD} ${INCLUDES} -c -o $@ $^

objects/flag_list.o: source/flag_list.cpp
	g++ ${STD} ${INCLUDES} -c -o $@ $^

objects/flag_manager.o: source/flag_manager.cpp
	g++ ${STD} ${INCLUDES} -c -o $@ $^
