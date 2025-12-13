#ifndef PREPARE_H
#define PREPARE_H

#include <stddef.h>
#include "bot.h"

typedef enum{
    TG_URL_OK = 0,

    /* Invalid inpput */
    TG_URL_ERR_NULL_ARG,
    TG_URL_ERR_EMPTY_TOKEN,

    TG_URL_ERR_BUFFER_TOO_SMALL, // buffer
    TG_URL_ERR_FORMAT // formatting

} tg_url_err_t;

tg_url_err_t build_url(char *url, size_t buffer_size, const bot_t *mybot);

#endif /* PREPARE_H*/
