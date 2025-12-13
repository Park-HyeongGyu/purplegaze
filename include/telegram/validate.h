#ifndef VALIDATE_H
#define VALIDATE_H

typedef enum{
    TG_VALID_MSG_OK = 0,
    TG_VALID_MSG_ERR_NULL, // message is not allocated
    TG_VALID_MSG_ERR_EMPTY, // messsage is empty
    TG_VALID_MSG_ERR_TOO_LONG // Telegram restricts message to be shorter than 4096 characters.
} tg_valid_msg_err_t;

tg_valid_msg_err_t validate_message(const char *message);

#endif /* VALIDATE_H */
