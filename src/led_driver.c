/*-------------------------------------------------------------------------*
 * File:  led_driver.c
 *-------------------------------------------------------------------------*
 * Description:
 */
/**
 *    @addtogroup led_driver
 *  @{
 *  @brief     TODO: Description
 *
 * @par Example code:
 * TOTO: Description of overall example code
 * @par
 * @code
 * TODO: Insert example code
 * @endcode
 */
/*-------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * Copyright (C) 2007-2017 Future Designs, Inc.
 *--------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include "led_driver.h"

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/
#define HEAD_LIGHT_DIM          (0x01)
#define HEAD_LIGHT_BRIGHT       (0x02)
#define TAIL_LIGHT_DIM          (0x10)
#define TAIL_LIGHT_BRIGHT       (0x20)
#define LEFT_TURN_SIGNALS       (0x44)
#define RIGHT_TURN_SIGNALS      (0x88)
#define ALL_TURN_SIGNALS        (0xCC)

#define STATE_ON                        0
#define STATE_OFF                       1
#define STATE_BLINK0                    2
#define STATE_BLINK1                    3

/*-------------------------------------------------------------------------*
 * Types:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Globals:
 *-------------------------------------------------------------------------*/
#ifdef PCA9551
T_LED_NXP_PCA9551_Workspace g_led_driver =
{ .iI2C = &g_sf_led_driver_i2c_device, .iSem = &g_led_driver_mutex, .iLEDsState =
{ 0 } };
#endif

led_driver_state_t g_led_state =
{ .headlights = LED_STATE_OFF, .taillights = LED_STATE_OFF, .turn_signals = LED_STATE_OFF, };
/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 * Function: TODO: Function Function Name, one for each function
 *---------------------------------------------------------------------------*/
/** TODO: Description of Function
 *
 *  @param [in] val1        TODO: param1
 *  @param [in] val2        TODO: param1
 *  @return                 TODO: Return Value
 *  @par Example Code:
 *  @code
 *  //TODO: provide example or remove if not needed
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
#define TESTING

void led_driver_update(void)
{
    rover_state_t state;
    rover_state_get (&state);

    if (state.light_state.headlight_switch_state == SWITCH_STATE_PARKING)
    {
        g_led_state.turn_signals = LED_STATE_ON;
    }
    else
    {
        g_led_state.turn_signals = LED_STATE_OFF;
    }

    if (state.light_state.headlight_switch_state == SWITCH_STATE_ON)
    {
        if (state.light_state.bright_switch_state)
        {
            g_led_state.headlights = LED_STATE_BRIGHT;
        }
        else
        {
            g_led_state.headlights = LED_STATE_ON;
        }

        g_led_state.taillights = LED_STATE_ON;
    }
    else
    {
        g_led_state.headlights = LED_STATE_OFF;
        g_led_state.taillights = LED_STATE_OFF;
    }

    switch (state.drive_state)
    {
        case DRIVE_STATE_STOPPED:
            g_led_state.taillights = LED_STATE_BRIGHT;
        break;

        case DRIVE_STATE_FORWARD:
            g_led_state.turn_signals = LED_STATE_OFF;
        break;

        case DRIVE_STATE_REVERSE:
            g_led_state.turn_signals = LED_STATE_OFF;
        break;

        case DRIVE_STATE_TURN_LEFT:
            g_led_state.turn_signals = LED_STATE_LEFT;
        break;

        case DRIVE_STATE_TURN_RIGHT:
            g_led_state.turn_signals = LED_STATE_RIGHT;
        break;

        case DRIVE_STATE_ROTATE_LEFT:
            g_led_state.turn_signals = LED_STATE_HAZARD;
        break;

        case DRIVE_STATE_ROTATE_RIGHT:
            g_led_state.turn_signals = LED_STATE_HAZARD;
        break;
    }

    if (state.light_state.hazard_switch_state)
    {
        g_led_state.turn_signals = LED_STATE_HAZARD;
    }

    led_driver_headlights_update ();
    led_driver_taillights_update ();
    led_driver_turn_signal_update ();
}

/*---------------------------------------------------------------------------*
 * Function: TODO: Function Function Name, one for each function
 *---------------------------------------------------------------------------*/
/** TODO: Description of Function
 *
 *  @param [in] val1        TODO: param1
 *  @param [in] val2        TODO: param1
 *  @return                 TODO: Return Value
 *  @par Example Code:
 *  @code
 *  //TODO: provide example or remove if not needed
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
void led_driver_headlights_update(void)
{
    switch (g_led_state.headlights)
    {
        case LED_STATE_OFF:
            g_led_driver.iLEDsState[0] = STATE_OFF;
            g_led_driver.iLEDsState[1] = STATE_OFF;
        break;
        case LED_STATE_ON:
            g_led_driver.iLEDsState[0] = STATE_ON;
            g_led_driver.iLEDsState[1] = STATE_OFF;
        break;
        case LED_STATE_BRIGHT:
            g_led_driver.iLEDsState[0] = STATE_ON;
            g_led_driver.iLEDsState[1] = STATE_ON;
        break;
        case LED_STATE_LEFT:
        break;
        case LED_STATE_RIGHT:
        break;
        case LED_STATE_HAZARD:
        break;
        default:
        break;
    }
    IUpdateLEDs(&g_led_driver);
}

/*---------------------------------------------------------------------------*
 * Function: TODO: Function Function Name, one for each function
 *---------------------------------------------------------------------------*/
/** TODO: Description of Function
 *
 *  @param [in] val1        TODO: param1
 *  @param [in] val2        TODO: param1
 *  @return                 TODO: Return Value
 *  @par Example Code:
 *  @code
 *  //TODO: provide example or remove if not needed
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
void led_driver_taillights_update(void)
{
    switch (g_led_state.taillights)
    {
        case LED_STATE_OFF:
            g_led_driver.iLEDsState[4] = STATE_OFF;
            g_led_driver.iLEDsState[5] = STATE_OFF;
        break;
        case LED_STATE_ON:
            g_led_driver.iLEDsState[4] = STATE_ON;
            g_led_driver.iLEDsState[5] = STATE_OFF;
        break;
        case LED_STATE_BRIGHT:
            g_led_driver.iLEDsState[4] = STATE_ON;
            g_led_driver.iLEDsState[5] = STATE_ON;
        break;
        case LED_STATE_LEFT:
        break;
        case LED_STATE_RIGHT:
        break;
        case LED_STATE_HAZARD:
        break;
        default:
        break;
    }
    IUpdateLEDs(&g_led_driver);
}
/*---------------------------------------------------------------------------*
 * Function: TODO: Function Function Name, one for each function
 *---------------------------------------------------------------------------*/
/** TODO: Description of Function
 *
 *  @param [in] val1        TODO: param1
 *  @param [in] val2        TODO: param1
 *  @return                 TODO: Return Value
 *  @par Example Code:
 *  @code
 *  //TODO: provide example or remove if not needed
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
void led_driver_turn_signal_update(void)
{
    switch (g_led_state.turn_signals)
    {
        case LED_STATE_OFF:
            /* Left turn signals */
            g_led_driver.iLEDsState[2] = STATE_OFF;
            g_led_driver.iLEDsState[6] = STATE_OFF;

            /* Right turn signals */
            g_led_driver.iLEDsState[3] = STATE_OFF;
            g_led_driver.iLEDsState[7] = STATE_OFF;
        break;
        case LED_STATE_ON:
            g_led_driver.iLEDsState[2] = STATE_ON;
            g_led_driver.iLEDsState[3] = STATE_ON;
            g_led_driver.iLEDsState[6] = STATE_ON;
            g_led_driver.iLEDsState[7] = STATE_ON;
        break;
        case LED_STATE_BRIGHT:
        break;
        case LED_STATE_LEFT:
            g_led_driver.iLEDsState[2] = STATE_BLINK1;
            g_led_driver.iLEDsState[6] = STATE_BLINK1;
            g_led_driver.iLEDsState[3] = STATE_OFF;
            g_led_driver.iLEDsState[7] = STATE_OFF;
        break;
        case LED_STATE_RIGHT:
            g_led_driver.iLEDsState[2] = STATE_OFF;
            g_led_driver.iLEDsState[6] = STATE_OFF;
            g_led_driver.iLEDsState[3] = STATE_BLINK1;
            g_led_driver.iLEDsState[7] = STATE_BLINK1;
        break;
        case LED_STATE_HAZARD:
            g_led_driver.iLEDsState[2] = STATE_BLINK1;
            g_led_driver.iLEDsState[3] = STATE_BLINK1;
            g_led_driver.iLEDsState[6] = STATE_BLINK1;
            g_led_driver.iLEDsState[7] = STATE_BLINK1;
        break;
        default:
        break;
    }
    IUpdateLEDs(&g_led_driver);
}

/*---------------------------------------------------------------------------*
 * Function: TODO: Function Function Name, one for each function
 *---------------------------------------------------------------------------*/
/** TODO: Description of Function
 *
 *  @param [in] val1        TODO: param1
 *  @param [in] val2        TODO: param1
 *  @return                 TODO: Return Value
 *  @par Example Code:
 *  @code
 *  //TODO: provide example or remove if not needed
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
void led_driver_init(void)
{
    ssp_err_t err = SSP_SUCCESS;

    err = g_sf_led_driver_i2c_device.p_api->open (g_sf_led_driver_i2c_device.p_ctrl, g_sf_led_driver_i2c_device.p_cfg);
    while (err)
    {

    }

    LED_NXP_PCA9551_InitializeWorkspace (&g_led_driver);
    LED_NXP_PCA9551_SetBlinkRate (&g_led_driver, 0, 17, 128);
    LED_NXP_PCA9551_SetBlinkRate (&g_led_driver, 1, 17, 128);
}
/*-------------------------------------------------------------------------*
 * End of File:  led_driver.c
 *-------------------------------------------------------------------------*/
