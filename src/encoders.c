/*-------------------------------------------------------------------------*
 * File:  encoders.c
 *-------------------------------------------------------------------------*
 * Description:
 */
/**
 *    @addtogroup encoders
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
#include "encoders.h"
#include "rover.h"

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Types:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Globals:
 *-------------------------------------------------------------------------*/
static encoder_memory_t g_encoder_memory[ENCODER_MEMORY_UNITS] =
{
{ 0 } };
static uint32_t enc_index = 0;

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
void encoders_update(encoder_memory_t * value)
{
    rover_state_t state;
    rover_command_t cmd;
    rover_speed_t speed;

    rover_state_get (&state);
    switch (state.drive_state)
    {
        case DRIVE_STATE_STOPPED:
        break;

        case DRIVE_STATE_FORWARD:

            /* Calculate the difference between the left and right encoder counters */
            g_encoder_memory[enc_index].counter_err_proportional = value->right_counter_value
                    - value->left_counter_value;

            /* Check to ensure valid indices */
            if (enc_index == 0)
            {
                /* Calculate the derivative */
                g_encoder_memory[enc_index].counter_err_derivative =
                        g_encoder_memory[enc_index].counter_err_proportional
                                - g_encoder_memory[ENCODER_MEMORY_UNITS - 1].counter_err_proportional;

                /* Add to running integral */
                g_encoder_memory[enc_index].counter_err_integral =
                        g_encoder_memory[ENCODER_MEMORY_UNITS - 1].counter_err_integral
                                + g_encoder_memory[enc_index].counter_err_proportional;
            }
            else
            {
                /* Calculate the derivative */
                g_encoder_memory[enc_index].counter_err_derivative =
                        g_encoder_memory[enc_index].counter_err_proportional
                                - g_encoder_memory[enc_index - 1].counter_err_proportional;

                /* Add to running integral */
                g_encoder_memory[enc_index].counter_err_integral = g_encoder_memory[enc_index - 1].counter_err_integral
                        + g_encoder_memory[enc_index].counter_err_proportional;

            }

            /* Calculate correction value (PID control) */
            g_encoder_memory[enc_index].counter_err_correction = (COUNTER_PID_P
                    * g_encoder_memory[enc_index].counter_err_proportional)
                    + (COUNTER_PID_I * g_encoder_memory[enc_index].counter_err_integral)
                    + (COUNTER_PID_D * g_encoder_memory[enc_index].counter_err_proportional);

            /* If an error value is greater than 0, update rover commands */
            if (g_encoder_memory[enc_index].counter_err_correction > 0)
            {
                rover_cmd_get (&cmd);
                rover_speed_get(&speed);

                if ((cmd.left_speed + g_encoder_memory[enc_index].counter_err_correction) <= speed.fast_speed_left)
                {
                    cmd.left_speed += g_encoder_memory[enc_index].counter_err_correction;
                }
                else
                {
                    cmd.right_speed -= g_encoder_memory[enc_index].counter_err_correction;
                }
            }
            if (g_encoder_memory[enc_index].counter_err_correction < 0)
            {
                if ((cmd.right_speed + g_encoder_memory[enc_index].counter_err_correction) <= speed.fast_speed_right)
                {
                    cmd.right_speed += g_encoder_memory[enc_index].counter_err_correction;
                }
                else
                {
                    cmd.left_speed -= g_encoder_memory[enc_index].counter_err_correction;
                }
            }

        break;

        case DRIVE_STATE_REVERSE:
            break;

        case DRIVE_STATE_TURN_LEFT:
        break;

        case DRIVE_STATE_TURN_RIGHT:
        break;

        case DRIVE_STATE_ROTATE_LEFT:
        break;

        case DRIVE_STATE_ROTATE_RIGHT:
        break;

        default:
        break;
    }

    if (enc_index == (ENCODER_MEMORY_UNITS - 1))
    {
        enc_index = 0;
    }
    else
    {
        enc_index++;
    }
}

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
void encoders_timer_mode_update(void)
{
    rover_command_t cmd;
    rover_cmd_get (&cmd);

    if (cmd.left_direction == FORWARD)
    {
        R_GPTB4->GTDNSR = 0; // Disable counting down on encoder rising edge for GPT11
        R_GPTB4->GTUPSR = 1; // Enable counting up on encoder rising edge for GPT11
    }
    else
    {
        R_GPTB4->GTUPSR = 0; // Disable counting up on encoder rising edge for GPT11
        R_GPTB4->GTDNSR = 1; // Enable counting down on encoder rising edge for GPT11
    }

    if (cmd.right_direction == FORWARD)
    {
        R_GPTB5->GTDNSR = 0; // Disable counting down on encoder rising edge for GPT12
        R_GPTB5->GTUPSR = 1; // Enable counting up on encoder rising edge for GPT12
    }
    else
    {
        R_GPTB5->GTUPSR = 0; // Disable counting up on encoder rising edge for GPT12
        R_GPTB5->GTDNSR = 1; // Enable counting down on encoder rising edge for GPT12
    }
}

void encoders_init(void)
{
    R_GPTB4->GTSTP_b.CSTOP11 = 1;
    R_GPTB4->GTSTP_b.CSTOP12 = 1;

    tx_thread_sleep(1);

    R_GPTB4->GTCLR_b.CCLR11 = 1;
    R_GPTB5->GTCLR_b.CCLR12 = 1;

    R_GPTB4->GTDNSR = 0; // Disable counting down on encoder rising edge for GPT11
    R_GPTB4->GTUPSR = 1; // Enable counting up on encoder rising edge for GPT11
    R_GPTB5->GTDNSR = 0; // Disable counting down on encoder rising edge for GPT12
    R_GPTB5->GTUPSR = 1; // Enable counting up on encoder rising edge for GPT12
}
/*-------------------------------------------------------------------------*
 * End of File:  encoders.c
 *-------------------------------------------------------------------------*/
