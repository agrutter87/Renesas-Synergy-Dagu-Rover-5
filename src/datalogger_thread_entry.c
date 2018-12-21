#include "datalogger.h"

/* Datalogger Thread entry function */
void datalogger_thread_entry(void)
{
    /* Variable for holding Synergy errors */
    ssp_err_t err = SSP_SUCCESS;

    /* Variable for holding commands from Bluetooth thread */
    uint8_t cmd;

    /* Configure and enable the timer used for measuring time between button presses */
    err = g_datalogger_timer.p_api->open (g_datalogger_timer.p_ctrl, g_datalogger_timer.p_cfg);
    while (err)
    {

    }

    /* Datalogger thread main loop */
    while (1)
    {

        /* Manual mode:   Sleep thread until notified that a new command has been sent from the app */
        /* Playback mode: Sleep thread until notified that it is time to update the Arduino with a new command */
        tx_semaphore_get (&g_datalogger_semaphore, TX_WAIT_FOREVER);

        /* Once notified, determine which mode its in */
        switch (rover_mode_get ())
        {
            /* If its in manual mode... */
            case MANUAL:
                /* Get the latest command from the Bluetooth thread */
                cmd = bt_cmd_get ();

                /* See if the command is a close or mode change command */
                switch (cmd)
                {
                    /* If it is, do not write it to the datalogger memory */
                    case 'x':
                    break;

                    case 'A':
                    break;

                    case 'a':
                    break;
                        /* Otherwise, write it to the datalogger memory */
                    default:
                        err = datalogger_cmd_write (cmd);
                    break;
                }
            break;

                /* If its in playback mode, read the next command from memory */
            case PLAYBACK:
                datalogger_cmd_read ();
            break;

            default:
            break;

                /* Return to start of loop and sleep until next notification */
        }

    }
}
