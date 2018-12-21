#include "led_driver_thread.h"
#include "led_driver.h"

/* LED Driver Thread entry function */
void led_driver_thread_entry(void)
{
    ssp_err_t err = SSP_SUCCESS;

    led_driver_init();

    while (1)
    {
        led_driver_update ();

        tx_semaphore_get (&g_led_driver_semaphore, TX_WAIT_FOREVER);
    }

}
