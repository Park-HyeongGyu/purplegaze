#ifndef PREPARE_H
#define PREPARE_H

#include <stddef.h>
#include "telegram/bot.h"

typedef enum{
    TG_URL_OK = 0,

    /* Invalid inpput */
    TG_URL_ERR_NULL_ARG,
    TG_URL_ERR_EMPTY_TOKEN,

    TG_URL_ERR_BUFFER_TOO_SMALL, // buffer
    TG_URL_ERR_FORMAT // formatting

} tg_url_err_t;

typedef enum{
    TG_PAYLORD_OK = 0,

    /* Invalid inpput */
    TG_PAYLORD_ERR_NULL_ARG,
    TG_PAYLORD_ERR_EMPTY_TOKEN,
    TG_PAYLORD_ERR_EMPTY_CHAT_ID,

    TG_PAYLORD_ERR_BUFFER_TOO_SMALL, // buffer
    TG_PAYLORD_ERR_FORMAT, // formatting
    TG_PAYLORD_ERR_ENCODING, // url_encoding
    TG_PAYLORD_ERR_ALLOC // dynamic allocation of messsage encoding buffer

} tg_paylord_err_t;

tg_url_err_t build_url(char *url, size_t buffer_size, const bot_t *mybot);
tg_paylord_err_t build_paylord(char *paylord,
    size_t paylord_buffer_size, const bot_t *mybot, const char *message);

#endif /* PREPARE_H*/
