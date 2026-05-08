#include <zephyr/kernel.h>
#include <zephyr/drivers/adc.h>

/* Use the alias 'adc0' defined in your overlay */
#define ADC_NODE DT_ALIAS(adc0)
static const struct device *adc_dev = DEVICE_DT_GET(ADC_NODE);

/* Hardware channel configuration */
static const struct adc_channel_cfg channel_cfg = {
    .gain             = ADC_GAIN_1,
    .reference        = ADC_REF_INTERNAL,
    .acquisition_time = ADC_ACQ_TIME_DEFAULT,
    .channel_id       = 2,  /* Must match 'reg' in overlay */
    .differential     = 0,
};

int main(void)
{
    int err;
    uint16_t buf;
    
    struct adc_sequence sequence = {
        .channels    = BIT(2), /* Bitmask for channel 0 */
        .buffer      = &buf,
        .buffer_size = sizeof(buf),
        .resolution  = 10,
    };

    if (!device_is_ready(adc_dev)) {
        return 0;
    }

    /* Minimal setup for one channel */
    err = adc_channel_setup(adc_dev, &channel_cfg);
    if (err < 0) {
        printf("Could not set adc, error: %d\n", err);
        return 0;
    }

    while (1) {
        err = adc_read(adc_dev, &sequence);
        if (err == 0) {
            /* Using printk is lighter than printf */
            printf("Raw Value: %u\n", buf);
        }

        k_msleep(1000);
    }

    return 0;
}
