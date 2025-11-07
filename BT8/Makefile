# -----------------------------
# Makefile for IPC Message Queues
# -----------------------------

# Compiler and flags
CXX      := g++
CXXFLAGS := -Wall -pthread -std=c++11

# Output binary name
TARGET_1   := task_worker
TARGET_2   := task_dispatcher
# Source files
SRCS_WORKER     := task_worker.c
SRCS_DISPATHCER := task_dispatcher.c
# Flags
FLAGS := -lrt
# Compile the program
all: $(SRCS_WORKER) $(SRCS_DISPATHCER)
	$(CXX) $(CXXFLAGS) $(SRCS_WORKER) -o $(TARGET_1) $(FLAGS)
	$(CXX) $(CXXFLAGS) $(SRCS_DISPATHCER) -o $(TARGET_2) $(FLAGS)
	@echo "[INFO] Build complete"

# Remove compiled files
clean:
	rm -f $(TARGET_1) $(TARGET_2)
	@echo "[INFO] Clean complete."

# Rebuild everything from scratch
rebuild: clean all