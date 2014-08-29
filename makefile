CXX = clang++
LFLAGS = -Wall -g -o Pong -lSDL2 -lSDL2_image -lSDL2_ttf -lpng
CXXFLAGS = -std=c++11 -Wall
SOURCES = $(wildcard src/**/*.cpp src/*.cpp)
OBJECTS = $(patsubst %.cpp,%.o,$(SOURCES))

p1: $(OBJECTS)
	$(CXX) $(LFLAGS) $(OBJECTS)

# Add dependencies to our main.o target
main.o: main.cpp
	$(CXX) $(CXXLFAGS) main.cpp

timer.o: timer.cpp timer.h
	$(CXX) $(CXXFLAGS) timer.cpp

clean:
	rm -f $(OBJECTS) Pong
