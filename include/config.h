#ifndef CONFIG_H
#define CONFIG_H

#define ENV_FILE ".env"

typedef enum{
    CFG_OK = 0,
    CFG_ERR_OPEN,
    CFG_ERR_TEMPLATE,
    CFG_ERR_PARSE,
    CFG_ERR_PERMISSION,
    CFG_ERR_VALIDATE
} cfg_err_t;

typedef struct{
    char bot_token[64];
    char chat_id[32];
    int battery_warning_level;
} config_t;

typedef struct{
    cfg_err_t err;
    config_t data;
} config_result_t;

/* load config from .env */
config_result_t config_load(void);

/* create template .env */
cfg_err_t config_create_template(void);

/* validate values */
cfg_err_t config_validate(const config_t *cfg);

/* check permission (should be 600) */
cfg_err_t config_check_permission(const char *path);

#endif
