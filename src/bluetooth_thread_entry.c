#include "bluetooth.h"

/* Bluetooth Thread entry function */
void bluetooth_thread_entry(void)
{
    /* Variable for holding errors */
    ssp_err_t err = SSP_SUCCESS;

    /* Performs hard reset on BT module */
    err = bt_reset ();
    while (err)
    {
        // trap on error
    }

    /* Waits for connection from app */
    err = bt_connect ();
    while (err)
    {
        // trap on error
    }

    /* Bluetooth thread main loop */
    while (1)
    {
        /* Waits for command to be sent from app */
        err = bt_read ();
        while (err)
        {
            // trap on error
        }

        /* Evaluates value sent from app if in manual mode or if the Bluetooth module loses connection */
        if ((!rover_mode_get ()) || (bt_cmd_get () == 'x') || (bt_cmd_get () == 'a'))
        {
            err = bt_cmd_eval ();
            while (err)
            {
                // trap on error
            }
        }
    }
}

