CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

TARGET = purplegaze

TELEGRAM_SRC = \
	src/telegram/telegram.c \
	src/telegram/telegram_api.c

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

LDFLAGS = -lcurl

# ====== Default build ======
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

# ====== Test build ======
test: clean $(TARGET)
	@echo "TESTING BUILD"

# ====== Pattern rule ======
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJ) $(TARGET)

