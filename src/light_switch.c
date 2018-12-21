/*-------------------------------------------------------------------------*
 * File:  light_switch.c
 *-------------------------------------------------------------------------*
 * Description:
 */
 /**
 *    @addtogroup light_switch
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
#include "light_switch.h"

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Types:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Globals:
 *-------------------------------------------------------------------------*/

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
void light_switch_state_update(void)
{
    rover_state_t rover_state;
    rover_state_get(&rover_state);

    switch(rover_state.light_state.headlight_switch_state)
    {
        case SWITCH_STATE_OFF:
            rover_state.light_state.headlight_switch_state = SWITCH_STATE_PARKING;
            break;
        case SWITCH_STATE_PARKING:
            rover_state.light_state.headlight_switch_state = SWITCH_STATE_ON;
            break;
        case SWITCH_STATE_ON:
            rover_state.light_state.headlight_switch_state = SWITCH_STATE_OFF;
            break;
    }

    rover_state_set(&rover_state);
    tx_semaphore_put(&g_led_driver_semaphore);
}

/*-------------------------------------------------------------------------*
 * End of File:  light_switch.c
 *-------------------------------------------------------------------------*/
