# Makefile for Portfolio Selection Backend

CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
TARGET = portfolio_backend
SOURCES = main.cpp common.cpp csv_io.cpp knapsack_2d.cpp knapsack_1d.cpp greedy.cpp fractional.cpp mst.cpp
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build complete: $(TARGET)"

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the program with default parameters
run: $(TARGET)
	./$(TARGET) stocks.csv 5000

# Run with custom parameters
# Usage: make run-custom INPUT=myfile.csv BUDGET=10000
run-custom: $(TARGET)
	./$(TARGET) $(INPUT) $(BUDGET)

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -f portfolio_results_*.csv algorithm_comparison.csv diversification_output.csv
	@echo "Clean complete"

# Clean only output CSV files
clean-output:
	rm -f portfolio_results_*.csv algorithm_comparison.csv diversification_output.csv
	@echo "Output files cleaned"

# Rebuild everything
rebuild: clean all

# Help target
help:
	@echo "Portfolio Selection Backend - Makefile Targets:"
	@echo "  make          - Build the project"
	@echo "  make run      - Build and run with default parameters (stocks.csv, budget=5000)"
	@echo "  make run-custom INPUT=file.csv BUDGET=amount - Run with custom parameters"
	@echo "  make clean    - Remove all build artifacts and output files"
	@echo "  make clean-output - Remove only CSV output files"
	@echo "  make rebuild  - Clean and rebuild"
	@echo "  make help     - Show this help message"

.PHONY: all run run-custom clean clean-output rebuild help
