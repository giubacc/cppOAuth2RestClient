SRC = 	./main.cpp\
		./properties_file_reader.cpp
		
OBJ = $(SRC:.cpp=.o)
OUTDIR = .
INCLUDES = 	-I.
			
CCFLAGS = -Wall -g
CCC = g++
LDLIBS= -lrestclient-cpp

.cpp.o:
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

default: restclient
	
all: clean restclient
	
restclient: $(OBJ)
	$(CCC) -o $(OUTDIR)/restclient $(OBJ) $(LDLIBS)

clean:
	rm -f $(OBJ) $(OUTDIR)/restclient
