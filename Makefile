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

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJ) $(TARGET)

