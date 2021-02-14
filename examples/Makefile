CC      = g++ -O3 -std=c++11
CFLAGS  = -Wall -Wextra -I../include
SRC_DIR = ./src
OBJ_DIR = ./obj
SOURCES = $(shell ls $(SRC_DIR)/*.cpp)
OBJS    = $(subst $(SRC_DIR),$(OBJ_DIR), $(SOURCES:.cpp=.o))
PQ_TARGET = priority_queue.out
SPQ_TARGET = stable_priority_queue.out
TARGET = $(PQ_TARGET) $(SPQ_TARGET)
DEPENDS = $(OBJS:.o=.d)

all: $(PQ_TARGET) $(SPQ_TARGET)

$(SPQ_TARGET): $(OBJ_DIR)/stable_priority_queue.o
	$(CC) -o $@ $<

$(PQ_TARGET): $(OBJ_DIR)/priority_queue.o
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
