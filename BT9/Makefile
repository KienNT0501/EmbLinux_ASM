# Compiler and flags
CXX      := g++
CXXFLAGS := -Wall -pthread -std=c++11
# Source
SRCS_PRODUCER := posix_shm_producer.c
SRCS_CONSUMER := posix_shm_consumer.c
# Libs
LIB := shared_memory.c
# Flag Libs:
LFLAG := -lrt
# Target
TARGET_1 := t1
TARGET_2 := t2
all: $(SRCS)
	$(CXX) $(SRCS_PRODUCER) $(LIB) -o $(TARGET_1) $(LFLAG)
	$(CXX) $(SRCS_CONSUMER) $(LIB) -o $(TARGET_2) $(LFLAG)
clean: $(TARGET_1) $(TARGET_2)
	rm -rf $(TARGET_1) $(TARGET_2)