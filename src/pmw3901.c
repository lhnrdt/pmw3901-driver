#define DT_DRV_COMPAT pixart_pmw3901

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/uart.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/sys/byteorder.h>
#include <zephyr/sys/util_macro.h>

#include <drivers/sensor/pmw3901.h>

LOG_MODULE_REGISTER(pmw3901, CONFIG_SENSOR_LOG_LEVEL);

struct pmw3901_data
{
    uint32_t x;
    uint32_t y;
};

struct pmw3901_config
{
};

static int pmw3901_init(const struct device *dev)
{
    const struct pmw3901_config *config = dev->config;
    struct pmw3901_data *data = dev->data;

    // Initialize the sensor here (e.g., configure GPIOs, UART, etc.)
    // For now, we will just set dummy values for x and y
    data->x = 0;
    data->y = 0;

    return 0;
}

static int jm101_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
    // only use dummy data for now
    struct pmw3901_data *data = dev->data;
    data->x = 0x12345678;
    data->y = 0x87654321;
    return 0;
}

static int jm101_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{

    struct pmw3901_data *data = dev->data;
    switch (chan)
    {
    case SENSOR_CHAN_X:
        val->val1 = data->x >> 16;
        val->val2 = data->x & 0xFFFF;
        break;
    case SENSOR_CHAN_Y:
        val->val1 = data->y >> 16;
        val->val2 = data->y & 0xFFFF;
        break;
    default:
        LOG_ERR("Unsupported channel %d", chan);
        return -ENOTSUP;
    }

    return 0;
}

static const struct sensor_driver_api pmw3901_driver_api = {
    .sample_fetch = pmw3901_sample_fetch,
    .channel_get = pmw3901_channel_get,
};

#define PMW3901_DEFINE(i)                                                                \
    static struct pmw3901_data pmw3901_data_##i = {                                      \
        .x = 0,                                                                          \
        .y = 0,                                                                          \
    };                                                                                   \
                                                                                         \
    static const struct pmw3901_config pmw3901_config_##i = {};                          \
                                                                                         \
    DEVICE_DT_INST_DEFINE(i, pmw3901_init, NULL, &pmw3901_data_##i, &pmw3901_config_##i, \
                          POST_KERNEL, CONFIG_SENSOR_INIT_PRIORITY, &pmw3901_driver_api);

DT_INST_FOREACH_STATUS_OKAY(PMW3901_DEFINE)