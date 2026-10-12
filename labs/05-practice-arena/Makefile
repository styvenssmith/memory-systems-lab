CC := cc

CFLAGS := -std=c17 -Wall -Wextra -Wpedantic -Werror \
          -g -O0 -fno-omit-frame-pointer \
          -fsanitize=address,undefined

TARGET := arena_test
SOURCES := arena.c test_arena.c

.PHONY: run clean

$(TARGET): $(SOURCES) arena.h
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)