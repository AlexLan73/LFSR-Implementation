# Makefile for LFSR Implementation
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
TARGET = lfsr_demo
SOURCES = lfsr.cpp lfsr_demo.cpp
HEADERS = lfsr.h

# Example targets
EXAMPLES = examples/basic_usage examples/sequence_analysis examples/performance_test
EXAMPLE_SOURCES = examples/basic_usage.cpp examples/sequence_analysis.cpp examples/performance_test.cpp

# Default target
all: $(TARGET) examples

# Build the main executable
$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

# Build examples
examples: $(EXAMPLES)

examples/basic_usage: examples/basic_usage.cpp lfsr.cpp lfsr.h
	$(CXX) $(CXXFLAGS) -o $@ $< lfsr.cpp

examples/sequence_analysis: examples/sequence_analysis.cpp lfsr.cpp lfsr.h
	$(CXX) $(CXXFLAGS) -o $@ $< lfsr.cpp

examples/performance_test: examples/performance_test.cpp lfsr.cpp lfsr.h
	$(CXX) $(CXXFLAGS) -o $@ $< lfsr.cpp

# Build test
test_lfsr: test_lfsr.cpp lfsr.cpp lfsr.h
	$(CXX) $(CXXFLAGS) -o test_lfsr test_lfsr.cpp lfsr.cpp

# Clean build artifacts
clean:
	rm -f $(TARGET) test_lfsr *.o
	rm -f $(EXAMPLES)

# Run the demonstration
run: $(TARGET)
	./$(TARGET)

# Run examples
run-examples: examples
	@echo "=== Running Basic Usage Example ==="
	./examples/basic_usage
	@echo "\n=== Running Sequence Analysis Example ==="
	./examples/sequence_analysis
	@echo "\n=== Running Performance Test Example ==="
	./examples/performance_test

# Run tests
test: test_lfsr
	./test_lfsr

# Debug build
debug: CXXFLAGS += -DDEBUG -O0
debug: $(TARGET) examples

# Release build
release: CXXFLAGS += -DNDEBUG -O3
release: $(TARGET) examples

# Install (copy to /usr/local/bin)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/

# Uninstall
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)

# Create distribution package
dist: clean
	@echo "Creating distribution package..."
	tar -czf lfsr-implementation.tar.gz \
		lfsr.h lfsr.cpp lfsr_demo.cpp test_lfsr.cpp \
		examples/ docs/ README.md QUICK_START.md LICENSE Makefile
	@echo "Distribution package created: lfsr-implementation.tar.gz"

# Help
help:
	@echo "Available targets:"
	@echo "  all          - Build the LFSR demonstration and examples (default)"
	@echo "  examples     - Build all example programs"
	@echo "  test_lfsr    - Build simple test program"
	@echo "  clean        - Remove build artifacts"
	@echo "  run          - Build and run the demonstration"
	@echo "  run-examples - Build and run all examples"
	@echo "  test         - Build and run tests"
	@echo "  debug        - Build with debug flags"
	@echo "  release      - Build with release optimization"
	@echo "  install      - Install to /usr/local/bin"
	@echo "  uninstall    - Remove from /usr/local/bin"
	@echo "  dist         - Create distribution package"
	@echo "  help         - Show this help message"

.PHONY: all examples test_lfsr clean run run-examples test debug release install uninstall dist help