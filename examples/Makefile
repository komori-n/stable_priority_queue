CC      = g++ -O3 -std=c++14
CFLAGS  = -Wall -Wextra -I../include
SRC_DIR = ./src
OBJ_DIR = ./obj
SOURCES = $(shell ls $(SRC_DIR)/*.cpp)
OBJS    = $(subst $(SRC_DIR),$(OBJ_DIR), $(SOURCES:.cpp=.o))
PQ_TARGET = priority_queue.out
SPQ_TARGET = stable_priority_queue.out
CBENCH_TARGET = copyable_bench.out
NCBENCH_TARGET = not_copyable_bench.out
TARGET = $(PQ_TARGET) $(SPQ_TARGET) $(CBENCH_TARGET) $(NCBENCH_TARGET)
DEPENDS = $(OBJS:.o=.d)

all: $(TARGET)

$(SPQ_TARGET): $(OBJ_DIR)/stable_priority_queue.o
	$(CC) -o $@ $<

$(PQ_TARGET): $(OBJ_DIR)/priority_queue.o
	$(CC) -o $@ $<

$(CBENCH_TARGET): $(OBJ_DIR)/copyable_bench.o
	$(CC) -o $@ $<

$(NCBENCH_TARGET): $(OBJ_DIR)/not_copyable_bench.o
	$(CC) -o $@ $<

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if [ ! -d $(OBJ_DIR) ]; \
		then echo "mkdir -p $(OBJ_DIR)"; mkdir -p $(OBJ_DIR); \
	fi
	$(CC) $(CFLAGS) -o $@ -c $< 

clean:
	$(RM) $(OBJS) $(TARGET) $(DEPENDS)

-include $(DEPENDS)

.PHONY: all clean
