CC = clang++
CXXFLAGS = -std=c++17
CFLAGS = -pthreads -Wall -g
INCLUDE = -Iinclude -I/usr/local/include 
LDFLAGS = -L/usr/local/lib
LIBRARIES = -L. -lhttpserver

DIR = src
TARGET = server
OBJFILES = main.o configs.o hello_world.o


all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) $(CFLAGS) $(LIBRARIES) -o $(TARGET).exe $(OBJFILES)
	
%.o: $(DIR)/%.cpp
	$(CC) $(CXXFLAGS) -c $^

clean:
	rm -f *.exe *.o




#$(ODIR)/%.o: $(SDIR)/%.cpp 
#	$(CC) -c $(INC) -o $@ $< $(CFLAGS) 


#ODIR = object file output directory
#SDIR = .cpp source file directory
#INC = list of -I flags
#CFLAGS = the usual