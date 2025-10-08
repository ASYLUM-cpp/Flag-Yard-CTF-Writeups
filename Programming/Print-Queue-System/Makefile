# Makefile for Secure Multi-Tier Print Queue Management System

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra
TARGET = print_queue
SRCS = main.cpp
HEADERS = PrintJob.hpp ArrayQueue.hpp LinkedListQueue.hpp

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRCS) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) audit.log

help:
	@echo "Available targets:"
	@echo "  all    - Build the print queue system (default)"
	@echo "  run    - Build and run the program"
	@echo "  clean  - Remove compiled binary and audit log"
	@echo "  help   - Display this help message"
