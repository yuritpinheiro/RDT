LIBS := -lsfml-audio -lsfml-graphics -lsfml-window -lsfml-system
CXX := g++

all: rdt

%.o: %.cpp
		$(CXX) -c $< -o $@

%.o: %.hpp
		$(CXX) -c $< -o $@

rdt: main.o
		@echo "** Building the RDT"
		$(CXX) -o rdt main.o $(LIBS)

clean:
		@echo "** Removing object files and executable..."
		rm -f rdt *.o

# install:
#         @echo '** Installing...'
#         cp rdt /usr/bin/

# uninstall:
#         @echo '** Uninstalling...'
#         $(RM) /usr/bin/thegame
