CPPFLAGS = -O3 -DDEBUG -g
	
CPPSOURCES = $(wildcard *.cpp)
    
all: aksim++
 
aksim++: $(CPPSOURCES:.cpp=.o)
	g++ -o $@ $^
 
%.o: %.cpp
	g++ -c $< $(CPPFLAGS) -o $@
    
clean:
	-rm -f *.o *~