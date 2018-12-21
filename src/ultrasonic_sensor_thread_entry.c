#include "ultrasonic_sensor_thread.h"
#include "ultrasonic_sensor.h"

timer_size_t ping_echo_time = 0;

/* Ultrasonic Sensor Thread entry function */
void ultrasonic_sensor_thread_entry(void)
{
    ssp_err_t err = SSP_SUCCESS;

    ultrasonic_sensor_init();

    ultrasonic_sensor_update();

    while (1)
    {
        tx_semaphore_get (&g_ultrasonic_sensor_semaphore, TX_WAIT_FOREVER);

        ultrasonic_sensor_update();
        ultrasonic_sensor_range_check();

        //g_ping_echo_timer.p_api->counterGet (g_ping_echo_timer.p_ctrl, &ping_echo_time);
        //g_ping_echo_timer.p_api->reset (g_ping_echo_timer.p_ctrl);
        tx_thread_sleep (50);
    }
}

void ping_callback(timer_callback_args_t * p_args)
{
    SSP_PARAMETER_NOT_USED(p_args);
    tx_semaphore_put (&g_ultrasonic_sensor_semaphore);
}
