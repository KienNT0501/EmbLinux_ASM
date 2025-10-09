# -----------------------------
# Makefile for P2P Chat Program
# -----------------------------

# Compiler and flags
CXX      := g++
CXXFLAGS := -Wall -pthread -std=c++11

# Output binary name
TARGET   := chat

# Source files
SRCS     := chat.cpp

# Default rule: build the program
all: $(TARGET)

# Compile the program
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)
	@echo "[INFO] Build complete. Run with: ./chat <port>"

# Run the program with a specific port (e.g. `make run PORT=5000`)
run: $(TARGET)
ifndef PORT
	@echo "[ERROR] Please specify a port. Example: make run PORT=5000"
else
	./$(TARGET) $(PORT)
endif

# Remove compiled files
clean:
	rm -f $(TARGET)
	@echo "[INFO] Clean complete."

# Rebuild everything from scratch
rebuild: clean all
