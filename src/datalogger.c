/*-------------------------------------------------------------------------*
 * File:  datalogger.c
 *-------------------------------------------------------------------------*
 * Description:
 */
/**
 *    @addtogroup datalogger
 *  @{
 *  @brief     Functions and variables used for reading and saving path data are located
 *  in this module.
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
#include "datalogger.h"

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/
#define MAX_COMMANDS 256

/*-------------------------------------------------------------------------*
 * Types:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Globals:
 *-------------------------------------------------------------------------*/
static uint16_t datalogger_index = 0;
static uint16_t final_index = 0;
static uint16_t interval = 0;
static datalogger_data_t data[MAX_COMMANDS];

/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 * Function: datalogger_cmd_write
 *---------------------------------------------------------------------------*/
/** The datalogger command write function checks to see if the datalogger index
 *  is 0. If it is, it starts the interval timer. If not, it gets the value of
 *  the interval timer and resets the timer.
 *  After handling the interval timer, it then stores the data into an array
 *  and increments the datalogger index and the maximum index.
 *
 *  @param [in] cmd         Command sent to the function to be saved
 *  @return                 Returns an SSP error
 *  @par Example Code:
 *  @code
 *  ssp_err_t err = SSP_SUCCESS;
 *  uint8_t cmd = some_command;
 *  err = datalogger_cmd_write(cmd);
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
ssp_err_t datalogger_cmd_write(uint8_t cmd)
{
    ssp_err_t err = SSP_SUCCESS;

    if (datalogger_index < MAX_COMMANDS)
    {
        data[datalogger_index].command = cmd;
        final_index = datalogger_index;

        if (!datalogger_index)
        {
            interval = 0;
            err = g_datalogger_timer.p_api->start (g_datalogger_timer.p_ctrl);
            if (err)
            {
                return err;
            }
        }
        else
        {
            err = g_datalogger_timer.p_api->reset (g_datalogger_timer.p_ctrl);
            if (err)
            {
                return err;
            }

            data[datalogger_index - 1].interval = interval;

            interval = 0;
        }

        datalogger_index++;
    }

    return err;
}

/*---------------------------------------------------------------------------*
 * Function: datalogger_cmd_read
 *---------------------------------------------------------------------------*/
/** The datalogger command read function first checks to see if the maximum
 *  index has been reached. If it has, it changes the rover back to manual mode.
 *  If not, it sets the period of the interval timer to trigger an interrupt at
 *  the next interval that was saved into the array by the datalogger_cmd_write
 *  function. It then resets and starts the timer just before setting the
 *  Bluetooth command and evaluating it. It then increments the datalogger index.
 *
 *  @return                 Returns an SSP error
 *  @par Example Code:
 *  @code
 *  ssp_err_t err = SSP_SUCCESS;
 *  err = datalogger_cmd_read();
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
ssp_err_t datalogger_cmd_read(void)
{
    ssp_err_t err = SSP_SUCCESS;

    if (datalogger_index <= final_index)
    {
        if (!data[datalogger_index].interval)
        {
            data[datalogger_index].interval = 1;
        }

        err = g_datalogger_timer.p_api->periodSet (g_datalogger_timer.p_ctrl, (data[datalogger_index].interval * 100),
                                                   TIMER_UNIT_PERIOD_MSEC);
        if (err)
        {
            return err;
        }

        err = g_datalogger_timer.p_api->reset (g_datalogger_timer.p_ctrl);
        if (err)
        {
            return err;
        }

        err = g_datalogger_timer.p_api->start (g_datalogger_timer.p_ctrl);
        if (err)
        {
            return err;
        }

        bt_cmd_set (data[datalogger_index].command);

        err = bt_cmd_eval ();
        if (err)
        {
            return err;
        }

        datalogger_index++;

        if (datalogger_index > final_index)
        {
            final_index = 0;
            tx_semaphore_put (&g_mode_semaphore);
        }
    }

    return err;
}

/*---------------------------------------------------------------------------*
 * Function: datalogger_mode_reset
 *---------------------------------------------------------------------------*/
/** The datalogger mode reset function stops and resets the timer not currently
 *  in use. This function is called when the mode is changed to prevent unwanted
 *  reads or writes to the datalogger. If the mode is changed to playback mode,
 *  the first datalogger read is initiated.
 *
 *  @par Example Code:
 *  @code
 *  ssp_err_t err = SSP_SUCCESS;
 *  err = datalogger_mode_reset();
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
ssp_err_t datalogger_mode_reset(void)
{
    ssp_err_t err = SSP_SUCCESS;
    datalogger_index = 0;

    err = g_datalogger_timer.p_api->stop (g_datalogger_timer.p_ctrl);
    if (err)
    {
        return err;
    }

    err = g_datalogger_timer.p_api->reset (g_datalogger_timer.p_ctrl);
    if (err)
    {
        return err;
    }

    if (rover_mode_get ()) // JUST SWITCHED TO PLAYBACK MODE
    {
        err = datalogger_cmd_read ();
        if (err)
        {
            return err;
        }
    }
    else
    {
        final_index = 0;
    }

    return err;
}

/*---------------------------------------------------------------------------*
 * Function: datalogger_down_timer_callback
 *---------------------------------------------------------------------------*/
/** This callback function is called when the down timer reaches its period
 *  value. This triggers the callback to notify the datalogger thread entry
 *  function to perform the next datalogger read. This function is called
 *  internally by SSP.
 */
/*---------------------------------------------------------------------------*/
void datalogger_timer_callback(timer_callback_args_t * p_args)
{
    SSP_PARAMETER_NOT_USED(p_args);

    if (rover_mode_get ())
    {
        g_datalogger_timer.p_api->stop (g_datalogger_timer.p_ctrl);
        tx_semaphore_put (&g_datalogger_semaphore);
    }
    else
    {
        interval++;
    }
}

/*-------------------------------------------------------------------------*
 * End of File:  datalogger.c
 *-------------------------------------------------------------------------*/
