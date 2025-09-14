SRC_PATH := $(PWD)/src
INC_PATH := ./inc
BIN_PATH := ./bin
LIB_PATH := ./lib
OBJ_PATH:= ./obj
SHARED_LIB_PATH := $(LIB_PATH)/shared
STATIC_LIB_PATH := $(LIB_PATH)/static
src_name := bstrutils
#export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$(pwd)/lib/shared
all: shared static
# 	gcc -c   $(SRC_PATH)/$(src_name).c   -o $(BIN_PATH)/static/lib.o
# 	gcc -c  -fPIC $(SRC_PATH)/$(src_name).c    -o $(BIN_PATH)/shared/lib.o
shared:
	gcc -c $(SRC_PATH)/main.c -o $(OBJ_PATH)/main.o
	gcc -c -fPIC $(SRC_PATH)/$(src_name).c -o $(OBJ_PATH)/$(src_name).o -I $(INC_PATH)/$(src_name).h
	gcc -shared $(OBJ_PATH)/$(src_name).o -o  $(SHARED_LIB_PATH)/libstr.so
	gcc  $(OBJ_PATH)/main.o -Llib/shared -lstr -o bin/use-shared-library -Wl,-rpath=$(PWD)/lib/shared
static:
	gcc -c $(SRC_PATH)/main.c -o $(OBJ_PATH)/main.o
	gcc -c   $(SRC_PATH)/$(src_name).c   -o $(OBJ_PATH)/$(src_name).o -I $(INC_PATH)/$(src_name).h
	ar rcs $(STATIC_LIB_PATH)/libstr.a $(OBJ_PATH)/$(src_name).o
	gcc   $(OBJ_PATH)/main.o  -Llib/static -lstr -o bin/use-static-library
clean:
	rm -rf $(BIN_PATH)/* $(OBJ_PATH)/*.o $(SHARED_LIB_PATH)/* $(STATIC_LIB_PATH)/*