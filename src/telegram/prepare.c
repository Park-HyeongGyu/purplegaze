#include <stdio.h> // snprintf
#include <stddef.h> // size_t
#include <string.h>
#include <stdlib.h>
#include "telegram/prepare.h"
#include "telegram/bot.h"

tg_url_err_t build_url(char *url, size_t buffer_size, const bot_t *mybot);

tg_paylord_err_t build_paylord(char *paylord,
    size_t paylord_buffer_size, const bot_t *mybot, const char *message);

static int url_encode(const char *input, char *output, size_t output_buffer_size);
static int is_url_safe(unsigned char c);

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
        size_t buffer_size, const bot_t *mybot){
    /* Validate argument */
    if (url == NULL || mybot == NULL){
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

tg_paylord_err_t build_paylord(char *paylord,
        size_t paylord_buffer_size,
        const bot_t *mybot, const char *message){
    /* Valudate argument */
    if (paylord == NULL || mybot == NULL || message == NULL){
        return TG_PAYLORD_ERR_NULL_ARG;
    }

    if (paylord_buffer_size == 0){
        return TG_PAYLORD_ERR_BUFFER_TOO_SMALL;
    }

    if (mybot->token[0] == '\0'){
        return TG_PAYLORD_ERR_EMPTY_TOKEN;
    }

    if (mybot->chat_id[0] == '\0'){
        return TG_PAYLORD_ERR_EMPTY_CHAT_ID;
    }

    char *encoded_message = NULL;
    size_t encoded_buffer_size;

    int encoding_result;
    int snprintf_result;

    /* Allocate message encoding buffer dynamically */
    encoded_buffer_size = strlen(message)*3 +1;
    encoded_message = malloc(encoded_buffer_size);
    if (encoded_message == NULL){
        return TG_PAYLORD_ERR_ALLOC;
    }

    /* URL encoded */
    encoding_result = url_encode(message,
                            encoded_message, encoded_buffer_size);
    if (encoding_result != 0){
        free(encoded_message);
        return TG_PAYLORD_ERR_ENCODING;
    }

    /* Build paylord */
    snprintf_result = snprintf(paylord, paylord_buffer_size,
                                "chat_id=%s&text=%s", 
                                mybot->chat_id, encoded_message);
    free(encoded_message);

    if (snprintf_result < 0){
        return TG_PAYLORD_ERR_FORMAT;
    }

    if ((size_t)snprintf_result >= paylord_buffer_size){
        return TG_PAYLORD_ERR_BUFFER_TOO_SMALL;
    }
    
    return TG_PAYLORD_OK;
}

/*
 * url_encode
 * Encode a string using application/x-www-form-urlencoded rules.
 * Although GPT made this fucntion, I think it is quietly well-designed;
 * hence I decided to use it.
 *
 * Returns:
 *   0  on success
 *  -1  on failure (output buffer too small or invalid arguments)
 */
int url_encode(const char *input, char *output, size_t output_buffer_size){
    static const char hex[] = "0123456789ABCDEF";
    size_t input_index = 0;
    size_t output_index = 0;

    if (input == NULL || output == NULL || output_buffer_size == 0){
        return -1;
    }

    while (input[input_index] != '\0') {
        unsigned char c = (unsigned char)input[input_index];

        /* Space -> '+' */
        if (c == ' ') {
            if (output_index + 1 >= output_buffer_size){
                return -1;
            }
            output[output_index++] = '+';
        }
        /* Unreserved characters */
        else if (is_url_safe(c)) {
            if (output_index + 1 >= output_buffer_size){
                return -1;
            }
            output[output_index++] = c;
        }
        /* Percent-encoding */
        else {
            if (output_index + 3 >= output_buffer_size){
                return -1;
            }
            output[output_index++] = '%';
            output[output_index++] = hex[c >> 4];
            output[output_index++] = hex[c & 0x0F];
        }

        input_index++;
    }

    if (output_index >= output_buffer_size){
        return -1;
    }

    output[output_index] = '\0';
    return 0;
}

/* RFC 3986 unreserved characters */
static int is_url_safe(unsigned char c)
{
    if ((c >= 'A' && c <= 'Z') ||
        (c >= 'a' && c <= 'z') ||
        (c >= '0' && c <= '9') ||
        c == '-' || c == '_' ||
        c == '.' || c == '~') {
        return 1;
    }
    return 0;
}
