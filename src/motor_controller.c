/*-------------------------------------------------------------------------*
 * File:  motor_controller.c
 *-------------------------------------------------------------------------*
 * Description:
 */
/**
 *    @addtogroup motor_controller
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
#include "motor_controller.h"

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/
#define MOTOR1_DIR_PIN   IOPORT_PORT_03_PIN_13
#define MOTOR2_DIR_PIN   IOPORT_PORT_03_PIN_14

#define MOTOR3_DIR_PIN   IOPORT_PORT_03_PIN_15
#define MOTOR4_DIR_PIN   IOPORT_PORT_04_PIN_14

#define FRONT_MOTOR_FORWARD_LEVEL   IOPORT_LEVEL_HIGH
#define FRONT_MOTOR_REVERSE_LEVEL   IOPORT_LEVEL_LOW

#define REAR_MOTOR_FORWARD_LEVEL    IOPORT_LEVEL_LOW
#define REAR_MOTOR_REVERSE_LEVEL    IOPORT_LEVEL_HIGH
/*-------------------------------------------------------------------------*
 * Types:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Globals:
 *-------------------------------------------------------------------------*/
static bool g_motor_controller_estop = false;

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
ssp_err_t motor_controller_update(void)
{
    ssp_err_t err = SSP_SUCCESS;

    if (g_motor_controller_estop)
    {
        /* Update speed for left motors */
        err = g_speed_pwm.p_api->dutyCycleSet (g_speed_pwm.p_ctrl, 0, TIMER_PWM_UNIT_PERCENT_X_1000, ROVER_LEFT_MOTORS);
        while (err)
        {

        }

        /* Update speed for right motors */
        err = g_speed_pwm.p_api->dutyCycleSet (g_speed_pwm.p_ctrl, 0, TIMER_PWM_UNIT_PERCENT_X_1000,
                                               ROVER_RIGHT_MOTORS);
        while (err)
        {

        }

    }
    else
    {
        rover_command_t cmd;
        rover_cmd_get (&cmd);

        if(g_motor_controller_estop)
        {
            cmd.left_speed = 0;
            cmd.right_speed = 0;
        }

        /* Update direction for left motors */
        if (cmd.left_direction == FORWARD)
        {
            err = g_ioport.p_api->pinWrite (MOTOR1_DIR_PIN, FRONT_MOTOR_FORWARD_LEVEL);
            err = g_ioport.p_api->pinWrite (MOTOR2_DIR_PIN, REAR_MOTOR_FORWARD_LEVEL);
        }
        else
        {
            err = g_ioport.p_api->pinWrite (MOTOR1_DIR_PIN, FRONT_MOTOR_REVERSE_LEVEL);
            err = g_ioport.p_api->pinWrite (MOTOR2_DIR_PIN, REAR_MOTOR_REVERSE_LEVEL);
        }

        /* Update direction for right motors */
        if (cmd.right_direction == FORWARD)
        {
            err = g_ioport.p_api->pinWrite (MOTOR3_DIR_PIN, FRONT_MOTOR_FORWARD_LEVEL);
            err = g_ioport.p_api->pinWrite (MOTOR4_DIR_PIN, REAR_MOTOR_FORWARD_LEVEL);
        }
        else
        {
            err = g_ioport.p_api->pinWrite (MOTOR3_DIR_PIN, FRONT_MOTOR_REVERSE_LEVEL);
            err = g_ioport.p_api->pinWrite (MOTOR4_DIR_PIN, REAR_MOTOR_REVERSE_LEVEL);
        }

        /* Update speed for left motors */
        err = g_speed_pwm.p_api->dutyCycleSet (g_speed_pwm.p_ctrl, cmd.left_speed, TIMER_PWM_UNIT_PERCENT_X_1000,
        ROVER_LEFT_MOTORS);
        while (err)
        {

        }

        /* Update speed for right motors */
        err = g_speed_pwm.p_api->dutyCycleSet (g_speed_pwm.p_ctrl, cmd.right_speed, TIMER_PWM_UNIT_PERCENT_X_1000,
        ROVER_RIGHT_MOTORS);
        while (err)
        {

        }
    }
    return err;
}

void motor_controller_estop_set(void)
{
    g_motor_controller_estop = true;
}

void motor_controller_estop_unset(void)
{
    g_motor_controller_estop = false;
}
/*-------------------------------------------------------------------------*
 * End of File:  motor_controller.c
 *-------------------------------------------------------------------------*/
