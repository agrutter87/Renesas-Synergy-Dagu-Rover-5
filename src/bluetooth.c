/*-------------------------------------------------------------------------*
 * File:  bluetooth.c
 *-------------------------------------------------------------------------*
 * Description:
 */
/**
 *    @addtogroup bluetooth
 *  @{
 *  @brief     Functions and variables relating to Bluetooth are located in this module
 *
 * @par Example code:
 * TODO: Description of overall example code
 * @par
 * @code
 * TODO: Insert example code
 * @endcode
 */
/*-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include "bluetooth.h"

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Types:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Globals:
 *-------------------------------------------------------------------------*/
static uint8_t bt_command; // Variable for holding data sent from app
static uint8_t dummy_command; // Variable for holding data sent from app during playback mode
static ioport_level_t bt_status = IOPORT_LEVEL_LOW;

/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 * Function: bt_reset
 *---------------------------------------------------------------------------*/
/** The Bluetooth reset function toggles power to the Pmod pin that the PmodBT2
 *  Bluetooth module is connected to.
 *
 *  @return                 Returns an SSP error
 *  @par Example Code:
 *  @code
 *  ssp_err_t err = SSP_SUCCESS;
 *  err = bt_reset();
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
ssp_err_t bt_reset(void)
{
    ssp_err_t err = SSP_SUCCESS;

    /* Turn off Bluetooth module */
    err = g_ioport.p_api->pinWrite (BLUETOOTH_POWER_PIN, IOPORT_LEVEL_LOW);
    if (err)
    {
        return err;
    }

    /* Puts the Bluetooth thread to sleep with the module OFF if the rover is in playback mode */
    tx_mutex_get (&g_mode_mutex, TX_WAIT_FOREVER);

    /* Turns power back on to the Bluetooth module */
    err = g_ioport.p_api->pinWrite (BLUETOOTH_POWER_PIN, IOPORT_LEVEL_HIGH);
    if (err)
    {
        return err;
    }

    /* Allows the mode to be changed back to playback mode if desired */
    tx_mutex_put (&g_mode_mutex);

    /* Wait */
    tx_thread_sleep (100);

    return err;
}

/*---------------------------------------------------------------------------*
 * Function: bt_connect
 *---------------------------------------------------------------------------*/
/** The Bluetooth connect function reads the Pmod pin connected to the PmodBT2
 *  status pin. While this pin is low, it periodically checks the status until
 *  the pin goes high, signifying that the Bluetooth module has successfully
 *  made a connection.
 *
 *  @return                 Returns an SSP error
 *  @par Example Code:
 *  @code
 *  ssp_err_t err = SSP_SUCCESS;
 *  err = bt_connect();
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
ssp_err_t bt_connect(void)
{
    ssp_err_t err = SSP_SUCCESS;

    /* Checks the status pin */
    err = g_ioport.p_api->pinRead (BLUETOOTH_STATUS_PIN, &bt_status);
    if (err)
    {
        return err;
    }

    /* While its LOW, keep checking */
    while (bt_status == IOPORT_LEVEL_LOW)
    {
        /* Checks the status pin */
        err = g_ioport.p_api->pinRead (BLUETOOTH_STATUS_PIN, &bt_status);
        if (err)
        {
            return err;
        }

        tx_thread_sleep (1);
    }
    return err;
}

/*---------------------------------------------------------------------------*
 * Function: bt_read
 *---------------------------------------------------------------------------*/
/** The Bluetooth read function first checks the rover mode.
 *
 *  If the rover is in manual control mode, the character sent to the Bluetooth
 *  module is read into the bt_command variable and the datalogger is notified
 *  that there is new data available.
 *
 *  If the rover is in playback mode, the character sent to the Bluetooth module
 *  is read into a dummy variable to flush the UART channel.
 *
 *  The dummy variable is then checked to see if it is the close command (x)
 *  and if it is, sets the bt_command to the value of the dummy variable. This
 *  allows the Bluetooth module to reset itself if the app is closed during
 *  playback operation.
 *
 *  @return                 Returns an SSP error
 *  @par Example Code:
 *  @code
 *  ssp_err_t err = SSP_SUCCESS;
 *  err = bt_read();
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
ssp_err_t bt_read(void)
{
    ssp_err_t err = SSP_SUCCESS;

    /* If the rover is in manual mode... */
    if (!rover_mode_get ())
    {
        /* Read the Bluetooth command into the bt_command variable */
        err = g_sf_bluetooth_comms.p_api->read (g_sf_bluetooth_comms.p_ctrl, &bt_command, 1, TX_WAIT_FOREVER);

        if (!((bt_command == 'a') || (bt_command == 'A')))
        {
            /* Notify the datalogger that new data is available to save to memory */
            tx_semaphore_put (&g_datalogger_semaphore);
        }
    }

    /* If the rover is in playback mode... */
    else
    {
        /* Read any Bluetooth commands into the dummy_command variable to flush the UART */
        err = g_sf_bluetooth_comms.p_api->read (g_sf_bluetooth_comms.p_ctrl, &dummy_command, 1, TX_WAIT_FOREVER);

        /* If that dummy command was an 'x', the Bluetooth module lost connection. The module
         * needs to be allowed to reset so the bt_command variable is updated to allow that.
         * Also, if the dummy command is an 'a', the mode is being switched back to manual and
         * that must be allowed through. */
        if ((dummy_command == 'x') || (dummy_command == 'a'))
        {
            bt_command = dummy_command;
        }
    }

    return err;
}

/*---------------------------------------------------------------------------*
 * Function: bt_cmd_eval
 *---------------------------------------------------------------------------*/
/** The Bluetooth command evaluate function checks the current value of the
 *  bt_command variable. If it is three x's, then it resets the Bluetooth module
 *  and scans until the connection is restored. If the rover is in playback mode,
 *  the reset is paused after the module is powered down.
 *
 *  @return                 Returns an SSP error
 *  @par Example Code:
 *  @code
 *  ssp_err_t err = SSP_SUCCESS;
 *  err = bt_cmd_eval();
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
ssp_err_t bt_cmd_eval(void)
{
    ssp_err_t err = SSP_SUCCESS;

    /* Evaluates the current command */
    switch (bt_command)
    {
        case 'x':
            /* Disconnect probably occurred because disconnect command is three x's. Double check: */
            err = bt_read ();
            if (err)
            {
                return err;
            }

            if (bt_command == 'x')
            {
                /* Very likely disconnect command was sent. Read again to flush */
                err = bt_read ();
                if (err)
                {
                    return err;
                }

                if (bt_command == 'x')
                {
                    /* connection terminated, reset BT (pauses within bt_reset() if in playback mode) */
                    err = bt_reset ();
                    if (err)
                    {
                        return err;
                    }

                    /* Try to reconnect to app */
                    err = bt_connect ();
                    if (err)
                    {
                        return err;
                    }

                    break;
                }
            }
        break;

            /* All other button commands are shown below */
        case PRESSED_FORWARD_CHAR:
            /* Updates a variable which holds each button state */
            rover_btn_update (UP, PRESSED);
            rover_cmd_update ();
        break;

        case RELEASED_FORWARD_CHAR:
            rover_btn_update (UP, RELEASED);
            rover_cmd_update ();
        break;

        case PRESSED_REVERSE_CHAR:
            rover_btn_update (DOWN, PRESSED);
            rover_cmd_update ();
        break;

        case RELEASED_REVERSE_CHAR:
            rover_btn_update (DOWN, RELEASED);
            rover_cmd_update ();
        break;

        case PRESSED_LEFT_CHAR:
            rover_btn_update (LEFT, PRESSED);
            rover_cmd_update ();
        break;

        case RELEASED_LEFT_CHAR:
            rover_btn_update (LEFT, RELEASED);
            rover_cmd_update ();
        break;

        case PRESSED_RIGHT_CHAR:
            rover_btn_update (RIGHT, PRESSED);
            rover_cmd_update ();
        break;

        case RELEASED_RIGHT_CHAR:
            rover_btn_update (RIGHT, RELEASED);
            rover_cmd_update ();
        break;

        case CHANGE_MODE_CHAR:
            rover_btn_update (UP, RELEASED);
            rover_btn_update (DOWN, RELEASED);
            rover_btn_update (LEFT, RELEASED);
            rover_btn_update (RIGHT, RELEASED);
            rover_cmd_update ();
            tx_semaphore_put (&g_mode_semaphore);
        break;
            /* Anything other than the desired buttons or close commands are ignored */
        default:
        break;
    }


    return err;
}

/*---------------------------------------------------------------------------*
 * Function: bt_cmd_set
 *---------------------------------------------------------------------------*/
/** The Bluetooth command set function allows the datalogger thread to access
 *  the local bt_command variable and set it to the value read from memory.
 *
 *  @param [in]  cmd        Command to update 'bt_command' to
 *  @param [out] bt_command Local variable to update to 'cmd' value
 *  @par Example Code:
 *  @code
 *  bt_cmd_set(data[datalogger_index].command);
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
void bt_cmd_set(uint8_t cmd)
{
    /* Sets the local variable to the value passed to it */
    bt_command = cmd;
}

/*---------------------------------------------------------------------------*
 * Function: bt_cmd_get
 *---------------------------------------------------------------------------*/
/** The Bluetooth command get function allows the datalogger thread to access
 *  the local bt_command variable and save it into a local array.
 *
 *  @return                 Returns the latest Bluetooth command
 *  @par Example Code:
 *  @code
 *  cmd = bt_cmd_get();
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
uint8_t bt_cmd_get(void)
{
    /* Returns the local variable bt_command to the calling function */
    return bt_command;
}

/*-------------------------------------------------------------------------*
 * End of File:  bluetooth.c
 *-------------------------------------------------------------------------*/
