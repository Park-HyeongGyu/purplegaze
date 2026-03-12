#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include "battery_monitor/reader.h"

#define SYSFS_POWER "/sys/class/power_supply"
#define PATHBUF 512

static int find_battery(char *path, size_t size)
{
    DIR *d = opendir(SYSFS_POWER);
    if(!d) return -1;

    struct dirent *ent;

    while((ent = readdir(d)) != NULL)
    {
        if(strcmp(ent->d_name,".") == 0 ||
           strcmp(ent->d_name,"..") == 0)
            continue;
        
        if(strlen(ent->d_name) > 100)
            continue;

        char type_file[PATHBUF];

        snprintf(type_file,sizeof(type_file),
                "%s/%s/type",
                SYSFS_POWER,
                ent->d_name);

        FILE *f = fopen(type_file,"r");
        if(!f) continue;

        char type[32];

        if(!fgets(type,sizeof(type),f))
        {
            fclose(f);
            continue;
        }

        fclose(f);

        if(strncmp(type,"Battery",7) == 0)
        {
            snprintf(path,size,
                    "%s/%s",
                    SYSFS_POWER,
                    ent->d_name);

            closedir(d);
            return 0;
        }
    }

    closedir(d);
    return -1;
}

battery_result_t bm_read_sysfs(void)
{
    battery_result_t result = {0};

    char path[PATHBUF];

    if (find_battery(path, sizeof(path)) != 0)
    {
        result.err = BM_ERR_NO_DEVICE;
        return result;
    }

    char file[PATHBUF];
    int n;

    /* ========================= */
    /* capacity read             */
    /* ========================= */

    n = snprintf(file, sizeof(file), "%s", path);
    if (n < 0 || (size_t)n >= sizeof(file))
    {
        result.err = BM_ERR_READ;
        return result;
    }

    if ((size_t)n + sizeof("/capacity") >= sizeof(file))
    {
        result.err = BM_ERR_READ;
        return result;
    }

    snprintf(file + n, sizeof(file) - n, "/capacity");

    FILE *f = fopen(file, "r");
    if (!f)
    {
        result.err = BM_ERR_READ;
        return result;
    }

    if (fscanf(f, "%d", &result.state.capacity) != 1)
    {
        fclose(f);
        result.err = BM_ERR_READ;
        return result;
    }

    fclose(f);

    /* ========================= */
    /* status read               */
    /* ========================= */

    n = snprintf(file, sizeof(file), "%s", path);
    if (n < 0 || (size_t)n >= sizeof(file))
    {
        result.err = BM_ERR_READ;
        return result;
    }

    if ((size_t)n + sizeof("/status") >= sizeof(file))
    {
        result.err = BM_ERR_READ;
        return result;
    }

    snprintf(file + n, sizeof(file) - n, "/status");

    f = fopen(file, "r");
    if (!f)
    {
        result.err = BM_ERR_READ;
        return result;
    }

    if (!fgets(result.state.status, sizeof(result.state.status), f))
    {
        fclose(f);
        result.err = BM_ERR_READ;
        return result;
    }

    fclose(f);

    /* Remove newline */
    result.state.status[strcspn(result.state.status, "\n")] = '\0';

    result.err = BM_OK;

    return result;
}
