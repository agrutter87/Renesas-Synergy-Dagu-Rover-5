/*-------------------------------------------------------------------------*
 * File:  ultrasonic_sensor.c
 *-------------------------------------------------------------------------*
 * Description:
 */
/**
 *    @addtogroup ultrasonic_sensor
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
#include "ultrasonic_sensor.h"

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Types:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Globals:
 *-------------------------------------------------------------------------*/
ultrasonic_sensor_mode_t g_ultrasonic_sensor_mode = PING_MODE_OFF;

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
void ultrasonic_sensor_update(void)
{
    rover_state_t rover_state;
    rover_state_get (&rover_state);
    ssp_err_t err = SSP_SUCCESS;

    static ultrasonic_sensor_mode_t current_mode = PING_MODE_OFF;

    switch (rover_state.drive_state)
    {
        case DRIVE_STATE_STOPPED:
            g_ultrasonic_sensor_mode = PING_MODE_ALTERNATE;
        break;

        case DRIVE_STATE_FORWARD:
            g_ultrasonic_sensor_mode = PING_MODE_FORWARD;
            current_mode = PING_MODE_FORWARD;
        break;

        case DRIVE_STATE_REVERSE:
            g_ultrasonic_sensor_mode = PING_MODE_REVERSE;
            current_mode = PING_MODE_REVERSE;
        break;

        case DRIVE_STATE_TURN_LEFT:
            g_ultrasonic_sensor_mode = PING_MODE_FORWARD;
            current_mode = PING_MODE_FORWARD;
        break;

        case DRIVE_STATE_TURN_RIGHT:
            g_ultrasonic_sensor_mode = PING_MODE_FORWARD;
            current_mode = PING_MODE_FORWARD;
        break;

        case DRIVE_STATE_ROTATE_LEFT:
            g_ultrasonic_sensor_mode = PING_MODE_ALTERNATE;
        break;

        case DRIVE_STATE_ROTATE_RIGHT:
            g_ultrasonic_sensor_mode = PING_MODE_ALTERNATE;
        break;

        default:
        break;
    }

//    g_ultrasonic_sensor_mode = PING_MODE_FORWARD; // TODO: remove

    switch (g_ultrasonic_sensor_mode)
    {
        case PING_MODE_OFF:
            // stop front ping PWM
            g_front_ping_pwm.p_api->stop (g_front_ping_pwm.p_ctrl);

            // stop rear ping PWM
            g_rear_ping_pwm.p_api->stop (g_front_ping_pwm.p_ctrl);
        break;

        case PING_MODE_FORWARD:
            // stop rear ping PWM
            g_rear_ping_pwm.p_api->stop (g_front_ping_pwm.p_ctrl);
            tx_thread_sleep (1);

            // start front ping PWM
            g_front_ping_pwm.p_api->start (g_front_ping_pwm.p_ctrl);
        break;

        case PING_MODE_REVERSE:
            // stop front ping PWM
            g_front_ping_pwm.p_api->stop (g_front_ping_pwm.p_ctrl);
            tx_thread_sleep (1);

            // start rear ping PWM
            g_rear_ping_pwm.p_api->start (g_rear_ping_pwm.p_ctrl);
        break;

        case PING_MODE_ALTERNATE:
            if (current_mode == PING_MODE_FORWARD)
            {
                current_mode = PING_MODE_REVERSE;

                // stop front ping PWM
                err = g_front_ping_pwm.p_api->stop (g_front_ping_pwm.p_ctrl);
                while (err)
                {

                }

                tx_thread_sleep (50);

                // start rear ping PWM
                err = g_rear_ping_pwm.p_api->start (g_rear_ping_pwm.p_ctrl);
                while (err)
                {

                }

                tx_thread_sleep (50);
                //     sleep one ping period

                tx_semaphore_put (&g_ultrasonic_sensor_semaphore);
                //     notify thread
            }
            else
            {
                current_mode = PING_MODE_FORWARD;
                // stop rear ping PWM
                err = g_rear_ping_pwm.p_api->stop (g_rear_ping_pwm.p_ctrl);
                while (err)
                {

                }

                tx_thread_sleep (50);

                // start front ping PWM
                err = g_front_ping_pwm.p_api->start (g_front_ping_pwm.p_ctrl);
                while (err)
                {

                }

                tx_thread_sleep (50);
                //     sleep one ping period

                tx_semaphore_put (&g_ultrasonic_sensor_semaphore);
                //     notify thread
            }
        break;
        default:
        break;
    }
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
void ultrasonic_sensor_init(void)
{
    ssp_err_t err = SSP_SUCCESS;

//    err = g_ping_echo_timer.p_api->open (g_ping_echo_timer.p_ctrl, g_ping_echo_timer.p_cfg);
//    while (err)
//    {
//
//    }

//    agt_counter_stop_wait (R_AGT1);
//    R_AGT1->AGTCR = (0xF0U | 1);
//
//
//        R_AGT1->AGTIOC_b.TOE = (uint8_t)(value & 0x01);
//
//        R_AGT1->AGTIOC_b.TEDGSEL = (uint8_t)(1 & 0x01);
//
//        //err = agt_interrupt_configure(p_ctrl, p_cfg);
//
//        R_AGT1->AGTMR1_b.TCK = (uint8_t)(6 & 0x07);
//
//        R_AGT1->AGTIOC_b.TEDGSEL = (uint8_t)(1 & 0x01);
//
//        R_AGT1->AGTMR1_b.TMOD = (uint8_t)(3 & 0x07);
//
//        /** Make sure period is valid, then set period */
////        p_ctrl->channel          = p_cfg->channel;
////        err = agt_period_set(R_AGT1, p_ctrl, p_cfg->period, p_cfg->unit);
//    }
//

    R_MSTP->MSTPCRD_b.MSTPD2 = 0x0;

    tx_thread_sleep (1);

    R_AGT1->AGTCR_b.TSTOP = 0x1;

    tx_thread_sleep (1);

    R_PFS->P204PFS_b.PMR = 0x0;
    R_PFS->P204PFS_b.PSEL = 0x1;

    tx_thread_sleep (1);

    R_PFS->P204PFS_b.PMR = 0x1;

    R_AGT1->AGTMR2_b.CKS = 0x0;

    tx_thread_sleep (1);

    R_AGT1->AGTMR1_b.TCK = 0x4;

    R_AGT1->AGTMR2_b.CKS = 0x0;
    R_AGT1->AGTIOC_b.TEDGSEL = 0x1;
    R_AGT1->AGTMR1_b.TMOD = 0x3;

    R_AGT1->AGTIOSEL_b.SEL = 0x0; // Select AGTIO_A as input
    R_AGT1->AGTIOSEL_b.TIES = 0x1; // Enable external event input during Software Standby mode.

    R_AGT1->AGTISR_b.EEPS = 0x1;
    R_AGT1->AGTCMSR = 0x0;

    R_AGT1->AGTCR_b.TEDGF = 0x0;
    R_AGT1->AGTCR_b.TUNDF = 0x0;
    R_AGT1->AGTCR_b.TCMAF = 0x0;
    R_AGT1->AGTCR_b.TCMBF = 0x0;

    tx_thread_sleep (1);

    //R_ICU->NMICLR = 0;

    tx_thread_sleep (1);

    R_AGT1->AGTCR_b.TSTART = 0x1;
//    R_MSTP->MSTPCRD_b.MSTPD2 = 0x1;

    tx_thread_sleep (1);

    err = g_front_ping_pwm.p_api->open (g_front_ping_pwm.p_ctrl, g_front_ping_pwm.p_cfg);
    while (err)
    {

    }

    err = g_rear_ping_pwm.p_api->open (g_rear_ping_pwm.p_ctrl, g_rear_ping_pwm.p_cfg);
    while (err)
    {

    }

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
void ultrasonic_sensor_range_check(void)
{
    uint16_t ping_echo_timer_value;
    static uint16_t prev_echo_timer_value;

    while (!((R_AGT1->AGTCR_b.TUNDF) || (R_AGT1->AGTCR_b.TEDGF)))
    {
        tx_thread_sleep (10);
    }

    if (R_AGT1->AGTCR_b.TEDGF)
    {
        // Read AGT counter
        ping_echo_timer_value = R_AGT1->AGT;

        R_AGT1->AGT_b.AGT = 0xFFFF;
        R_AGT1->AGTCR_b.TEDGF = 0x0;
    }

    if (R_AGT1->AGTCR_b.TUNDF)
    {
        R_AGT1->AGTCR_b.TUNDF = 0x0;
    }

    if ((ping_echo_timer_value > 65300) && (prev_echo_timer_value <= 65300))
    {
        motor_controller_estop_set ();
        motor_controller_update ();
    }
    else if ((ping_echo_timer_value <= 65300) && (prev_echo_timer_value > 65300))
    {
        motor_controller_estop_unset ();
        motor_controller_update ();
    }

    prev_echo_timer_value = ping_echo_timer_value;
}

/*-------------------------------------------------------------------------*
 * End of File:  ultrasonic_sensor.c
 *-------------------------------------------------------------------------*/
