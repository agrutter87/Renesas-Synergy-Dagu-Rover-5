#include "motor_controller.h"

/* Motor Controller Thread entry function */
void motor_controller_thread_entry(void)
{
    /* Variable for holding errors */
    ssp_err_t err = SSP_SUCCESS;

    /* Open and start PWM */
    err = g_speed_pwm.p_api->open(g_speed_pwm.p_ctrl, g_speed_pwm.p_cfg);
    while(err)
    {

    }


    /* Turn off left motors */
    err = g_speed_pwm.p_api->dutyCycleSet(g_speed_pwm.p_ctrl, 0, TIMER_PWM_UNIT_PERCENT_X_1000, ROVER_LEFT_MOTORS);
    while(err)
    {

    }

    /* Turn off right motors */
    err = g_speed_pwm.p_api->dutyCycleSet(g_speed_pwm.p_ctrl, 0, TIMER_PWM_UNIT_PERCENT_X_1000, ROVER_RIGHT_MOTORS);
    while(err)
    {

    }


    /* Arduino thread main loop */
    while (1)
    {
        /* Wait for a notification */
        tx_semaphore_get(&g_motor_controller_semaphore, TX_WAIT_FOREVER);

        /* Sends the latest command variables to the Arduino via UART */
        err = motor_controller_update ();
        while(err)
        {

        }

        /* Notify the encoder system that the control has changed */
//TODO:        tx_semaphore_put(&g_encoders_semaphore);

    }
}
