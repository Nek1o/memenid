CC = clang

TARGET_EXEC := out

SRC_DIRS := ./src
BUILD_DIR := ./build
INC_DIRS := $(shell find $(SRC_DIRS) -type d)

SRCS := $(shell find $(SRC_DIRS) -name *.c)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)