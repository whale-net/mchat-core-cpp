CC			= g++
LDFLAGS		= -L/home/alex/boost/include -pthread
CFLAGS		= -I/home/alex/boost/include -c -g -Wall
# TODO, define header for server.cpp so we can compile server.cpp separate from main
SOURCES		= server_main.cpp server.cpp endpoints/example.cpp
OBJECTS		= $(SOURCES:.cpp=.o)
EXECUTABLE	= server

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o
	rm $(EXECUTABLE)
