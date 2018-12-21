/*-------------------------------------------------------------------------*
 * File:  encoders.h
 *-------------------------------------------------------------------------*
 * Description:
 */
 /**
 *	@file 	encoders.h
 *  @brief 	TODO: Short description
 *
 *	TODO: Description of file
 */
/*-------------------------------------------------------------------------*/
#ifndef ENCODERS_H_
#define ENCODERS_H_

/*--------------------------------------------------------------------------
 * Copyright (C) 2007-2017 Future Designs, Inc.
 *--------------------------------------------------------------------------
 *
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include "encoders_thread.h"
#include "rover.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/
#define ENCODER_MEMORY_UNITS (3)
#define COUNTER_PID_P (1)
#define COUNTER_PID_I (0)
#define COUNTER_PID_D (0)

#define INCHES_PER_COUNT (0.0935)

/*-------------------------------------------------------------------------*
 * Types:
 *-------------------------------------------------------------------------*/
typedef struct
{
    uint32_t left_counter_value;
    uint32_t right_counter_value;
    int32_t counter_err_expected;
    int32_t counter_err_proportional;
    int32_t counter_err_derivative;
    int32_t counter_err_integral;
    int32_t counter_err_correction;
}encoder_memory_t;


/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/

/** TODO: Description, one for each prototype
 *
 *  @param [in] val1        TODO: param1
 *  @param [in] val2        TODO: param1
 *  @return                 TODO: Return Value
 */
void encoders_update(encoder_memory_t * memory);
void encoders_timer_mode_update(void);
#ifdef __cplusplus
}
#endif

#endif // ENCODERS_H_
/*-------------------------------------------------------------------------*
 * End of File:  encoders.h
 *-------------------------------------------------------------------------*/
