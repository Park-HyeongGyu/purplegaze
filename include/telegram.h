#ifndef TELEGRAM_H
#define TELEGRAM_H

#include "telegram/transport.h"

typedef struct{
    char token[64];
    char chat_id[32];
} bot_t;

#endif /* TELEGRAM_H */

