#ifndef TELEGRAM_H
#define TELEGRAM_H

#include "telegram/bot.h"

typedef enum{
    TG_OK = 0,

    TG_ERR_VALID, // Validation failed
    TG_ERR_PREP, // Rreparation of paylord and url failed
    TG_ERR_TRANS // Transporting http failed
} tg_err_t;

tg_err_t send_telegram(const char *message, const bot_t *mybot);

#endif /* TELEGRAM_H */
