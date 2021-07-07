CC = clang
DEBUG = -g -O0 
LD = -lssl -lcrypto

TARGET_EXEC := out

SRC_DIRS := ./src
BUILD_DIR := ./build
INC_DIRS := $(shell find $(SRC_DIRS) -type d)

SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(DEBUG) $(CFLAGS) -c $< -o $@


$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(DEBUG) $(LD) $(OBJS) -o $@

.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)