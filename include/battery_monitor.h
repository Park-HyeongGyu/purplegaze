#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

typedef enum{
    BM_OK = 0,
    BM_ERR_NO_DEVICE,
    BM_ERR_READ,
    BM_ERR_UDEV
} bm_err_t;

typedef struct{
    int capacity;
    char status[32];
} battery_state_t;

typedef struct{
    bm_err_t err;
    battery_state_t state;
} battery_result_t;

/* init module */
bm_err_t battery_init(void);

/* check if event happened */
bm_err_t battery_poll_event(battery_result_t *result);

/* read state manually */
battery_result_t battery_read_state(void);

#endif
