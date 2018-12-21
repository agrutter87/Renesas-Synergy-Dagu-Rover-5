/*-------------------------------------------------------------------------*
 * File:  led_driver.h
 *-------------------------------------------------------------------------*
 * Description:
 */
 /**
 *	@file 	led_driver.h
 *  @brief 	TODO: Short description
 *
 *	TODO: Description of file
 */
/*-------------------------------------------------------------------------*/
#ifndef LED_DRIVER_H_
#define LED_DRIVER_H_

#define PCA9551

/*--------------------------------------------------------------------------
 * Copyright (C) 2007-2017 Future Designs, Inc.
 *--------------------------------------------------------------------------
 *
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/

#include "LED_NXP_PCA9551.h"
#include "rover.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Types:
 *-------------------------------------------------------------------------*/
typedef enum
{
    LED_STATE_OFF = 0,
    LED_STATE_ON,
    LED_STATE_BRIGHT,
    LED_STATE_LEFT,
    LED_STATE_RIGHT,
    LED_STATE_HAZARD,
} led_state_t;

typedef struct
{
    led_state_t headlights;
    led_state_t taillights;
    led_state_t turn_signals;
} led_driver_state_t;

/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/

/** TODO: Description, one for each prototype
 *
 *  @param [in] val1        TODO: param1
 *  @param [in] val2        TODO: param1
 *  @return                 TODO: Return Value
 */
void led_driver_init(void);
void led_driver_update(void);

void led_driver_headlights_update(void);
void led_driver_taillights_update(void);
void led_driver_turn_signal_update(void);

#ifdef __cplusplus
}
#endif

#endif // LED_DRIVER_H_
/*-------------------------------------------------------------------------*
 * End of File:  led_driver.h
 *-------------------------------------------------------------------------*/
