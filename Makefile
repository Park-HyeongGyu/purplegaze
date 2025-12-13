CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

TARGET = purplegaze

TELEGRAM_SRC = \
	src/telegram/telegram.c

STATUS_SRC = \
	src/status/status.c

CONFIG_SRC = \
	src/config/config.c

SRC = \
	src/main.c \
	$(TELEGRAM_SRC) \
	$(STATUS_SRC) \
	$(CONFIG_SRC)

OBJ = $(SRC:.c=.o)

# ====== Default build ======
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# ====== Test build ======
TEST_FLAGS = 
test: CFLAGS += $(TEST_FLAGS) -g
test: clean $(TARGET)
	@echo "Test flags: $(TEST_FLAGS)"

# ====== Pattern rule ======
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJ) $(TARGET)

