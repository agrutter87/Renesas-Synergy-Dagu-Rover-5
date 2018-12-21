#include "power_monitor_thread.h"

#define LOW_VOLTAGE (0x0FFF)

// ADC0 data buffer indices
#define BATTERY_VOLTAGE_INDEX   (5)
#define M1_CURRENT_INDEX        (3)
#define M4_CURRENT_INDEX        (0)
#define MCU_VOLTAGE_INDEX       (29)

// ADC1 data buffer indices
#define M2_CURRENT_INDEX        (0)
#define M3_CURRENT_INDEX        (1)

static double g_battery_voltage = 0;
static double g_mcu_voltage = 0;
static double g_m1_current = 0;
static double g_m2_current = 0;
static double g_m3_current = 0;
static double g_m4_current = 0;

/* Battery Monitor Thread entry function */
void power_monitor_thread_entry(void)
{
    ssp_err_t err = SSP_SUCCESS;

    err = g_sf_power_monitor_adc_periodic0.p_api->start(g_sf_power_monitor_adc_periodic0.p_ctrl);
    while (err)
    {

    }

    err = g_sf_power_monitor_adc_periodic1.p_api->start(g_sf_power_monitor_adc_periodic1.p_ctrl);
    while (err)
    {

    }

    while(1)
    {
        tx_semaphore_get(&g_battery_monitor_semaphore, TX_WAIT_FOREVER);

        g_battery_voltage = (g_battery_monitor_adc_buffer0[BATTERY_VOLTAGE_INDEX] * 0.00080586) + 0.5;
        g_mcu_voltage = (g_battery_monitor_adc_buffer0[MCU_VOLTAGE_INDEX] * 0.00080586) + 0.5;
        g_m1_current = (g_battery_monitor_adc_buffer0[M1_CURRENT_INDEX] * 0.00080586);
        g_m2_current = (g_battery_monitor_adc_buffer1[M2_CURRENT_INDEX] * 0.00080586);
        g_m3_current = (g_battery_monitor_adc_buffer1[M3_CURRENT_INDEX] * 0.00080586);
        g_m4_current = (g_battery_monitor_adc_buffer0[M4_CURRENT_INDEX] * 0.00080586);

        // if(g_battery_voltage <= 2.8) rover_emergency_stop();
        // if(g_mcu_voltage <= 2.8) rover_emergency_stop();
        // if(g_m1_current >= 2) rover_emergency_stop();
        // if(g_m2_current >= 2) rover_emergency_stop();
        // if(g_m3_current >= 2) rover_emergency_stop();
        // if(g_m4_current >= 2) rover_emergency_stop();

    }
}

void g_battery_monitor_adc_callback(sf_adc_periodic_callback_args_t *p_args)
{
    SSP_PARAMETER_NOT_USED(p_args);

    tx_semaphore_put(&g_battery_monitor_semaphore);
}
