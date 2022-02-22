CC = clang++
CFLAGS = -std=c++17 -pthreads -Wall -g
INCLUDE = -Iinclude -I/usr/local/include 
LDFLAGS = -L/usr/local/lib
LIBRARIES = -L. -lhttpserver

TARGET = main
CONFIG = configs

.PHONY: all clean


all: $(TARGET)

$(TARGET): $(TARGET).cpp $(CONFIG).cpp
	$(CC) $(INCLUDE) $(LDFLAGS) $(CFLAGS) $(LIBRARIES) -o server.exe $(TARGET).cpp $(CONFIG).cpp
	
clean:
	rm -f *.exe *.o