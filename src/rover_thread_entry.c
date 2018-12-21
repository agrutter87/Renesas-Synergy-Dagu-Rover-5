#include "rover.h"

void rover_thread_entry(void);

/* Rover Thread entry function */
void rover_thread_entry(void)
{
    ssp_err_t err = SSP_SUCCESS;

    g_ioport.p_api->pinWrite(IOPORT_PORT_04_PIN_02, IOPORT_LEVEL_HIGH);//RED LED
    g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_12, IOPORT_LEVEL_HIGH); //BLUE LED
    g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_13, IOPORT_LEVEL_LOW);//GREEN LED

    /* Rover thread main loop */
    while(1)
    {
        /* Wait until notified of button press */
        tx_semaphore_get(&g_mode_semaphore, TX_WAIT_FOREVER);

        /* Once notified, determine the current mode and toggle it */
        if (rover_mode_get())
        {
            g_ioport.p_api->pinWrite(IOPORT_PORT_04_PIN_02, IOPORT_LEVEL_HIGH); //RED LED
            g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_12, IOPORT_LEVEL_HIGH); //BLUE LED
            g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_13, IOPORT_LEVEL_LOW); //GREEN LED

            /* Sets to manual mode */
            rover_mode_set(MANUAL);

            /* Allows the Bluetooth module to turn back on and start scanning */
            tx_mutex_put(&g_mode_mutex);
        }
        else
        {
            /* Lets the Bluetooth module turn off if the app is closed */
            tx_mutex_get(&g_mode_mutex, TX_WAIT_FOREVER);

            g_ioport.p_api->pinWrite(IOPORT_PORT_04_PIN_02, IOPORT_LEVEL_LOW); //RED LED
            g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_12, IOPORT_LEVEL_HIGH); //BLUE LED
            g_ioport.p_api->pinWrite(IOPORT_PORT_07_PIN_13, IOPORT_LEVEL_HIGH); //GREEN LED

            /* Sets to playback mode */
            rover_mode_set(PLAYBACK);
        }

        /* Reset the datalogger */
        err = datalogger_mode_reset();
        while(err)
        {
            // trap on error
        }
    }
}

