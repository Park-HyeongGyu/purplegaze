#include <stdlib.h>
#include <string.h>
#include "telegram/validate.h"

tg_valid_msg_err_t validate_message(const char *message){
    const size_t MAX_MSG_LEN = 4096;

    if (message == NULL){
        return TG_VALID_MSG_ERR_NULL;
    }

    if (message[0] == '\0'){
        return TG_VALID_MSG_ERR_EMPTY;
    }

    if (strlen(message) > MAX_MSG_LEN){
        return TG_VALID_MSG_ERR_TOO_LONG;
    }

    return TG_VALID_MSG_OK;
}
