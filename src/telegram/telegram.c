#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "telegram.h"
#include "telegram/bot.h"
#include "telegram/prepare.h"
#include "telegram/transport_http.h"
#include "telegram/validate.h"

tg_err_t send_telegram(const char *message, const bot_t *mybot){
    /* Validate Argument */
    if (message == NULL || mybot == NULL){
        return TG_ERR_VALID;
    }

    tg_valid_msg_err_t valid_result;
    tg_url_err_t url_result;
    tg_paylord_err_t paylord_result;
    tg_http_err_t http_result;

    char *url = NULL, *paylord = NULL;

    size_t msg_len, url_buffer_size, paylord_buffer_size;

    /* Validate Message */
    valid_result = validate_message(message);
    if (valid_result != TG_VALID_MSG_OK){
        return TG_ERR_VALID;
    }
    msg_len = strlen(message);
    // I would like to validate mybot but I will postpone it
    // because it is 4:00 am.

    /* Preparation - Dynamic allocation of url */
    url_buffer_size = strlen("https://api.telegram.org/bot") +
                    strlen(mybot->token) + strlen("/sendMessage") + 1;
    url = malloc(url_buffer_size);
    if (url == NULL){
        return TG_ERR_PREP;
    }
    url_result = build_url(url, url_buffer_size, mybot);
    if (url_result != TG_URL_OK){
        free(url);
        return TG_ERR_PREP;
    }

    /* Preparation - Dynamic allocation of paylord */
    paylord_buffer_size = strlen("chat_id=&text=") +
                    strlen(mybot->chat_id) + msg_len*3 + 1;
    paylord = malloc(paylord_buffer_size);
    if (paylord == NULL){
        return TG_ERR_PREP;
    }
    paylord_result = build_paylord(paylord, paylord_buffer_size, mybot, message);
    if (paylord_result != TG_PAYLORD_OK){
        free(paylord);
        return TG_ERR_PREP;
    }
    
    /* Transport https */
    http_result = send_http(url, paylord);

    free(url);
    free(paylord);

    if (http_result != TG_HTTP_OK){
        return TG_ERR_TRANS;
    }

    return TG_OK;
}

