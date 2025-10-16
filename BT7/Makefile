# -----------------------------
# Makefile for P2P Chat Program
# -----------------------------

# Compiler and flags
CXX      := g++
CXXFLAGS := -Wall -pthread -std=c++11

# Output binary name
TARGET   := mypipe

# Source files
SRCS     := mypipe.c

# Default rule: build the program
all: $(TARGET)

# Compile the program
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)
	@echo "[INFO] Build complete. Run with: ./mypipe"

# Remove compiled files
clean:
	rm -f $(TARGET)
	@echo "[INFO] Clean complete."

# Rebuild everything from scratch
rebuild: clean all
