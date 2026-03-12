#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "config.h"

#define LINEBUF 256

static void trim_newline(char *s)
{
    s[strcspn(s,"\r\n")] = '\0';
}

static int parse_int_strict(const char *s, int *out)
{
    char *end;
    long v;

    if(!s || !out)
        return -1;

    v = strtol(s, &end, 10);

    if(end == s || *end != '\0')
        return -1;

    if(v < 0 || v > 100)
        return -1;

    *out = (int)v;

    return 0;
}

static int split_key_value(char *line, char **key, char **value)
{
    char *eq = strchr(line,'=');

    if(!eq)
        return -1;

    *eq = '\0';

    *key = line;
    *value = eq + 1;

    trim_newline(*value);

    return 0;
}

cfg_err_t config_create_template(void)
{
    FILE *f = fopen(ENV_FILE,"w");
    if(!f)
        return CFG_ERR_TEMPLATE;

    fprintf(f,"BOT_TOKEN=\n");
    fprintf(f,"CHAT_ID=\n");
    fprintf(f,"BATTERY_WARNING_LEVEL=20\n");

    fclose(f);

    return CFG_OK;
}

cfg_err_t config_check_permission(const char *path)
{
    struct stat st;

    if(stat(path,&st) != 0)
        return CFG_ERR_OPEN;

    if((st.st_mode & 077) != 0)
        return CFG_ERR_PERMISSION;

    return CFG_OK;
}

cfg_err_t config_validate(const config_t *cfg)
{
    if(cfg->bot_token[0] == '\0')
        return CFG_ERR_VALIDATE;

    if(cfg->chat_id[0] == '\0')
        return CFG_ERR_VALIDATE;

    if(cfg->battery_warning_level <= 0 ||
       cfg->battery_warning_level > 100)
        return CFG_ERR_VALIDATE;

    return CFG_OK;
}

config_result_t config_load(void)
{
    config_result_t result = {0};
    FILE *f;
    char line[LINEBUF];
    config_t cfg = {0};

    f = fopen(ENV_FILE,"r");

    if(!f)
    {
        if(config_create_template() != CFG_OK)
        {
            result.err = CFG_ERR_TEMPLATE;
            return result;
        }

        result.err = CFG_ERR_OPEN;
        return result;
    }

    fclose(f);

    result.err = config_check_permission(ENV_FILE);
    if(result.err != CFG_OK)
        return result;

    f = fopen(ENV_FILE,"r");
    if(!f)
    {
        result.err = CFG_ERR_OPEN;
        return result;
    }

    while(fgets(line,sizeof(line),f))
    {
        char *key;
        char *value;

        trim_newline(line);

        if(line[0]=='\0' || line[0]=='#')
            continue;

        if(split_key_value(line,&key,&value)!=0)
        {
            result.err = CFG_ERR_PARSE;
            fclose(f);
            return result;
        }

        if(strcmp(key,"BOT_TOKEN")==0)
        {
            snprintf(cfg.bot_token,
                     sizeof(cfg.bot_token),
                     "%s",
                     value);
        }
        else if(strcmp(key,"CHAT_ID")==0)
        {
            snprintf(cfg.chat_id,
                     sizeof(cfg.chat_id),
                     "%s",
                     value);
        }
        else if(strcmp(key,"BATTERY_WARNING_LEVEL")==0)
        {
            if(parse_int_strict(value,
                &cfg.battery_warning_level) != 0)
            {
                result.err = CFG_ERR_PARSE;
                fclose(f);
                return result;
            }
        }
    }

    fclose(f);

    if(config_validate(&cfg)!=CFG_OK)
    {
        result.err = CFG_ERR_VALIDATE;
        return result;
    }

    result.data = cfg;
    result.err = CFG_OK;

    return result;
}
