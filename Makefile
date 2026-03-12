CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

TARGET = purplegaze

TELEGRAM_SRC = \
	src/telegram/telegram.c \
	src/telegram/transport_http.c \
	src/telegram/prepare.c \
	src/telegram/validate.c

STATUS_SRC = \
	src/status/status.c

CONFIG_SRC = \
	src/config/config.c

BATTERY_MONITOR_SRC = \
	src/battery_monitor/reader.c \
	src/battery_monitor/battery_monitor.c

SRC = \
	src/main.c \
	$(TELEGRAM_SRC) \
	$(STATUS_SRC) \
	$(CONFIG_SRC) \
	$(BATTERY_MONITOR_SRC)

OBJ = $(SRC:.c=.o)

LDFLAGS = -lcurl
LIBS = -ludev

# ====== Default build ======
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS) $(LIBS)

# ====== Test build ======
test: clean $(TARGET)
	@echo "TESTING BUILD"

# ====== Pattern rule ======
src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -f $(OBJ) $(TARGET)

