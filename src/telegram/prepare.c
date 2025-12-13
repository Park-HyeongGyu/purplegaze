#include <stdio.h> // snprintf
#include <stddef.h> // size_t
#include "prepare.h"
#include "bot.h"

tg_url_err_t build_url(char *url, size_t buffer_size, const bot_t *mybot);

/*
 * build_send_url
 * buidl an url, which is doomed to be an argument of the function
 * sending a message to telegram. It utilized snprintf internally.
 * 
 * Parameters:
 *  it's trivial
 * Returns:
 *  tg_url_err_t indicates whether building url successed or failed.
*/
tg_url_err_t build_url(char *url, 
                          size_t buffer_size,
                          const bot_t *mybot){
    /* Argument validation */
    if (url == NULL || mybot == NULL || mybot->token == NULL){
        return TG_URL_ERR_NULL_ARG;
    }

    if (mybot->token[0] == '\0'){
        return TG_URL_ERR_EMPTY_TOKEN;
    }

    /* Build URL */
    int snprintf_result = snprintf(url, buffer_size,
        "https://api.telegram.org/bot%s/sendMessage", mybot->token);
    
    /* Check whether snprinf failed */
    if (snprintf_result < 0){
        return TG_URL_ERR_FORMAT;
    }

    /* Buffer size check (checking overflow) */
    if ((size_t)snprintf_result >= buffer_size){
        return TG_URL_ERR_BUFFER_TOO_SMALL;
    }

    return TG_URL_OK;
}
