CC = clang++
CXXFLAGS = -std=c++17
CFLAGS = -pthreads -Wall -g $(shell pkg-config --cflags xmlsec1)
INCLUDE = -Iinclude -I/usr/local/include 
LDFLAGS = -L/usr/local/lib
LIBRARIES = -L. -lhttpserver $(shell pkg-config --libs xmlsec1)

DIR = src
TARGET = server
#OBJFILES = main.o configs.o logger.o
SRCFILES = $(DIR)/main.cpp $(DIR)/configs.cpp $(DIR)/logger.cpp $(DIR)/file_signer.cpp


all: $(TARGET)
#rm *.o

$(TARGET): $(OBJFILES)
	$(CC) $(CXXFLAGS) $(INCLUDE) $(LDFLAGS) $(CFLAGS) $(LIBRARIES) -o $(TARGET).exe $(SRCFILES)
	
#%.o: $(DIR)/%.cpp
#	$(CC) $(CXXFLAGS) -c $^

clean:
	rm -f *.exe *.o *.txt




#$(ODIR)/%.o: $(SDIR)/%.cpp 
#	$(CC) -c $(INC) -o $@ $< $(CFLAGS) 


#ODIR = object file output directory
#SDIR = .cpp source file directory
#INC = list of -I flags
#CFLAGS = the usual