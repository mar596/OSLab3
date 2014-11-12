CC = g++
LFLAGS = -std=c++11
CFLAGS = -Wall -w 

SOURCES = Task.cpp Operating_System.cpp main.cpp 
OBJECTS = $(SOURCES:.cpp = .o)

EXECNAME = lab_3

all: $(EXECNAME)

$(EXECNAME): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(EXECNAME) $(OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $(LFLAGS) -c $< -o $@

clean:
	$(RM) *.o *~ $(EXECNAME)