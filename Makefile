CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall
TARGET = portfolio
TEST_TARGET = test_runner
SRC_DIR = src
SOURCES = $(SRC_DIR)/main.cpp $(SRC_DIR)/knapsack.cpp $(SRC_DIR)/io.cpp $(SRC_DIR)/utils.cpp
TEST_SOURCES = $(SRC_DIR)/tests.cpp $(SRC_DIR)/knapsack.cpp $(SRC_DIR)/io.cpp $(SRC_DIR)/utils.cpp
ZIP_NAME = portfolio-selection.zip

.PHONY: all run test clean zip help

all: $(TARGET)
	@echo "Build complete. Run './$(TARGET)' to start the program."

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCES)

$(TEST_TARGET): $(TEST_SOURCES)
	$(CXX) $(CXXFLAGS) -o $(TEST_TARGET) $(TEST_SOURCES)

run: $(TARGET)
	./$(TARGET)

test: $(TEST_TARGET)
	@echo "Running unit tests..."
	./$(TEST_TARGET)
	@echo ""
	@echo "All tests completed!"

clean:
	rm -f $(TARGET) $(TEST_TARGET) $(ZIP_NAME)
	@echo "Clean complete."

zip: clean all test
	@echo "Creating submission archive..."
	@rm -f $(ZIP_NAME)
	zip -r $(ZIP_NAME) \
		src/ \
		data/ \
		docs/ \
		Makefile \
		LICENSE \
		README.md \
		STEP2_COMPLETE.md \
		STEP3_COMPLETE.md \
		STEP4_COMPLETE.md \
		PROJECT_SUMMARY.md \
		QUICK_REFERENCE.md \
		TEST_GUIDE.md \
		-x "*.exe" "*.o" "*~" ".git/*"
	@echo ""
	@echo "Archive created: $(ZIP_NAME)"
	@echo "Ready for submission!"

help:
	@echo "Portfolio Selection System - Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  make          - Compile the main program"
	@echo "  make run      - Compile and run the program"
	@echo "  make test     - Compile and run unit tests"
	@echo "  make clean    - Remove compiled binaries"
	@echo "  make zip      - Create submission archive (builds, tests, then zips)"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Quick start:"
	@echo "  1. make test  - Verify everything works"
	@echo "  2. make run   - Try the program"
	@echo "  3. make zip   - Create submission file"
