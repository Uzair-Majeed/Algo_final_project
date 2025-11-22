# Makefile for Disaster Relief Optimization System (C++)
# Author: CS2009 Project
# Date: November 2025

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wextra
DEBUG_FLAGS = -std=c++17 -g -Wall -Wextra -DDEBUG
LDFLAGS = 

# Target executable
TARGET = disaster_relief

# Source files
SOURCES = main.cpp graph.cpp algorithms.cpp solver.cpp benchmark.cpp
HEADERS = graph.h algorithms.h solver.h benchmark.h

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCES) $(HEADERS)
	@echo "Compiling and linking $(TARGET)..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)
	@echo "Build complete!"
	@echo "Run with: ./$(TARGET)"

# Compile source files to object files (for incremental builds)
%.o: %.cpp $(HEADERS)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug build
debug: CXXFLAGS = $(DEBUG_FLAGS)
debug: clean $(TARGET)
	@echo "Debug build complete!"

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(TARGET) $(OBJECTS) *.o
	@echo "Clean complete!"

# Clean all generated files
cleanall: clean
	@echo "Cleaning generated files..."
	rm -f *.json sample_*.json large_dataset.json benchmark_results.json
	@echo "Clean all complete!"

# Run sample problem
run-sample: $(TARGET)
	./$(TARGET) sample

# Run benchmark
run-benchmark: $(TARGET)
	./$(TARGET) benchmark

# Run all tests
run-all: $(TARGET)
	./$(TARGET) all

# Run interactive mode
run: $(TARGET)
	./$(TARGET)

# Install (optional - copies to /usr/local/bin)
install: $(TARGET)
	@echo "Installing $(TARGET) to /usr/local/bin..."
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Installation complete!"

# Uninstall
uninstall:
	@echo "Uninstalling $(TARGET)..."
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstall complete!"

# Help
help:
	@echo "Disaster Relief Optimization System - Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  make              - Build the project"
	@echo "  make debug        - Build with debug symbols"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make cleanall     - Remove all generated files"
	@echo "  make run-sample   - Build and run sample problem"
	@echo "  make run-benchmark- Build and run benchmarks"
	@echo "  make run-all      - Build and run all tests"
	@echo "  make run          - Build and run interactive mode"
	@echo "  make install      - Install to system (requires sudo)"
	@echo "  make uninstall    - Remove from system (requires sudo)"
	@echo "  make help         - Show this help message"
	@echo ""
	@echo "Compiler: $(CXX)"
	@echo "Flags: $(CXXFLAGS)"

# Check coding style (requires clang-format)
format:
	@echo "Formatting source code..."
	@if command -v clang-format >/dev/null 2>&1; then \
		clang-format -i $(SOURCES) $(HEADERS); \
		echo "Formatting complete!"; \
	else \
		echo "clang-format not found. Install it to use this target."; \
	fi

# Static analysis (requires cppcheck)
analyze:
	@echo "Running static analysis..."
	@if command -v cppcheck >/dev/null 2>&1; then \
		cppcheck --enable=all --suppress=missingIncludeSystem $(SOURCES); \
	else \
		echo "cppcheck not found. Install it to use this target."; \
	fi

# Generate documentation (requires doxygen)
docs:
	@echo "Generating documentation..."
	@if command -v doxygen >/dev/null 2>&1; then \
		doxygen Doxyfile; \
		echo "Documentation generated in docs/"; \
	else \
		echo "doxygen not found. Install it to use this target."; \
	fi

.PHONY: all clean cleanall debug run run-sample run-benchmark run-all install uninstall help format analyze docs

