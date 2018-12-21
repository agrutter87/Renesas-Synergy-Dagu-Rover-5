/*-------------------------------------------------------------------------*
 * File:  ultrasonic_sensor.h
 *-------------------------------------------------------------------------*
 * Description:
 */
 /**
 *	@file 	ultrasonic_sensor.h
 *  @brief 	TODO: Short description
 *
 *	TODO: Description of file
 */
/*-------------------------------------------------------------------------*/
#ifndef ULTRASONIC_SENSOR_H_
#define ULTRASONIC_SENSOR_H_

/*--------------------------------------------------------------------------
 * Copyright (C) 2007-2017 Future Designs, Inc.
 *--------------------------------------------------------------------------
 *
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/

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
    PING_MODE_OFF = 0,
    PING_MODE_FORWARD,
    PING_MODE_REVERSE,
    PING_MODE_ALTERNATE
} ultrasonic_sensor_mode_t;
/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/

/** TODO: Description, one for each prototype
 *
 *  @param [in] val1        TODO: param1
 *  @param [in] val2        TODO: param1
 *  @return                 TODO: Return Value
 */
void ultrasonic_sensor_update(void);

/** TODO: Description, one for each prototype
 *
 *  @param [in] val1        TODO: param1
 *  @param [in] val2        TODO: param1
 *  @return                 TODO: Return Value
 */
void ultrasonic_sensor_init(void);

/** TODO: Description, one for each prototype
 *
 *  @param [in] val1        TODO: param1
 *  @param [in] val2        TODO: param1
 *  @return                 TODO: Return Value
 */
void ultrasonic_sensor_range_check(void);

#ifdef __cplusplus
}
#endif

#endif // ULTRASONIC_SENSOR_H_
/*-------------------------------------------------------------------------*
 * End of File:  ultrasonic_sensor.h
 *-------------------------------------------------------------------------*/
