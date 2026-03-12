#include <libudev.h>
#include <sys/select.h>
#include <unistd.h>
#include <string.h>

#include "battery_monitor.h"
#include "battery_monitor/reader.h"

static struct udev *udev_ctx = NULL;
static struct udev_monitor *udev_mon = NULL;
static int udev_fd = -1;

bm_err_t battery_init(void)
{
    udev_ctx = udev_new();
    if(!udev_ctx)
        return BM_ERR_UDEV;

    udev_mon =
        udev_monitor_new_from_netlink(
            udev_ctx,"udev");

    udev_monitor_filter_add_match_subsystem_devtype(
        udev_mon,"power_supply",NULL);

    udev_monitor_enable_receiving(udev_mon);

    udev_fd = udev_monitor_get_fd(udev_mon);

    return BM_OK;
}

battery_result_t battery_read_state(void)
{
    return bm_read_sysfs();
}

bm_err_t battery_poll_event(battery_result_t *result)
{
    fd_set fds;

    FD_ZERO(&fds);
    FD_SET(udev_fd,&fds);

    struct timeval tv;

    tv.tv_sec = 0;
    tv.tv_usec = 0;

    int ret = select(udev_fd+1,&fds,NULL,NULL,&tv);

    if(ret <= 0)
        return BM_ERR_UDEV;

    struct udev_device *dev =
        udev_monitor_receive_device(udev_mon);

    if(!dev)
        return BM_ERR_UDEV;

    const char *name = udev_device_get_sysname(dev);
    
    if(name &&
       (strstr(name,"BAT") ||
        strstr(name,"AC")  ||
        strstr(name,"ADP")))
    {
        *result = bm_read_sysfs();
    }

    udev_device_unref(dev);

    return BM_OK;
}
