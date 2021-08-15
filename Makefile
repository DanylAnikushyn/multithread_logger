CXX = g++
CXXFLAGS = -Wall -std=c++17
LDFLAGS = -pthread
TARGET = main
OBJS = main.o logger.o  


all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -rf $(OBJS) $(TARGET)

.PHONY: all clean test

