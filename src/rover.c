/*-------------------------------------------------------------------------*
 * File:  rover.c
 *-------------------------------------------------------------------------*
 * Description:
 */
/**
 *    @addtogroup rover
 *  @{
 *  @brief     Functions and variables relating to the Rover5 as a whole are
 *             located in this module.
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
static rover_command_t g_rover_commands =
{ .left_speed = 0, .right_speed = 0 };

static rover_app_button_t g_app_buttons =
{ .buttons_pressed = 0 };

static rover_mode_t g_rover_mode = MANUAL;
static rover_state_t g_rover_state =
        {
         .drive_state = DRIVE_STATE_STOPPED,
         .light_state =
         {
          .headlight_switch_state = SWITCH_STATE_OFF,
          .bright_switch_state = false,
          .hazard_switch_state = false
         },
        };

static rover_speed_t g_rover_speed =
{
 .fast_speed_left = FAST_SPEED,
 .fast_speed_right = FAST_SPEED,
 .slow_speed_left = SLOW_SPEED,
 .slow_speed_right = SLOW_SPEED
};


/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*
 * Function: rover_cmd_update
 *---------------------------------------------------------------------------*/
/** The rover command update function updates the local variables used to hold
 *  the desired speeds and direction that will be communicated to the Arduino.
 *  Since there are four buttons, each with two states, there are a total of 16
 *  possible button state combinations. Each of these button state combos
 *  are used to update the command variables that will be sent to the Arduino.
 *
 *  @return                 Returns an SSP error
 *  @par Example Code:
 *  @code
 *  ssp_err_t err = SSP_SUCCESS;
 *  err = rover_cmd_update();
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
ssp_err_t rover_cmd_update(void)
{
    ssp_err_t err = SSP_SUCCESS;

    /* Looks at the entire state of all buttons */
    switch (g_app_buttons.buttons_pressed)
    {
        case 0: // (0000) Nothing is pressed
            g_rover_commands.left_speed = 0;
            g_rover_commands.right_speed = 0;
            g_rover_state.drive_state = DRIVE_STATE_STOPPED;
        break;

        case 1: // (0001) Only UP pressed
            g_rover_commands.left_speed = g_rover_speed.fast_speed_left;
            g_rover_commands.right_speed = g_rover_speed.fast_speed_right;
            g_rover_commands.left_direction = FORWARD;
            g_rover_commands.right_direction = FORWARD;
            g_rover_state.drive_state = DRIVE_STATE_FORWARD;
        break;

        case 2: // (0010) Only DOWN pressed
            g_rover_commands.left_speed = g_rover_speed.fast_speed_left;
            g_rover_commands.right_speed = g_rover_speed.fast_speed_right;
            g_rover_commands.left_direction = REVERSE;
            g_rover_commands.right_direction = REVERSE;
            g_rover_state.drive_state = DRIVE_STATE_REVERSE;
        break;

        case 3: // (0011) Both UP and DOWN pressed
            g_rover_commands.left_speed = 0;
            g_rover_commands.right_speed = 0;
            g_rover_state.drive_state = DRIVE_STATE_STOPPED;
        break;

        case 4: // (0100) Only LEFT pressed
            g_rover_commands.left_speed = g_rover_speed.fast_speed_left;
            g_rover_commands.right_speed = g_rover_speed.fast_speed_right;
            g_rover_commands.left_direction = REVERSE;
            g_rover_commands.right_direction = FORWARD;
            g_rover_state.drive_state = DRIVE_STATE_ROTATE_LEFT;
        break;

        case 5: // (0101) Both LEFT and UP pressed
            g_rover_commands.left_speed = 0;
            g_rover_commands.right_speed = g_rover_speed.fast_speed_right;
            g_rover_commands.right_direction = FORWARD;
            g_rover_state.drive_state = DRIVE_STATE_TURN_LEFT;
        break;

        case 6: // (0110) Both LEFT and DOWN pressed
            g_rover_commands.left_speed = 0;
            g_rover_commands.right_speed = g_rover_speed.fast_speed_right;
            g_rover_commands.right_direction = REVERSE;
            g_rover_state.drive_state = DRIVE_STATE_REVERSE;
        break;

        case 7: // (0111) LEFT, UP, and DOWN pressed
            g_rover_commands.left_speed = g_rover_speed.fast_speed_left;
            g_rover_commands.right_speed = g_rover_speed.fast_speed_right;
            g_rover_commands.left_direction = REVERSE;
            g_rover_commands.right_direction = FORWARD;
            g_rover_state.drive_state = DRIVE_STATE_ROTATE_LEFT;
        break;

        case 8: // (1000) Only RIGHT pressed
            g_rover_commands.left_speed = g_rover_speed.fast_speed_left;
            g_rover_commands.right_speed = g_rover_speed.fast_speed_right;
            g_rover_commands.left_direction = FORWARD;
            g_rover_commands.right_direction = REVERSE;
            g_rover_state.drive_state = DRIVE_STATE_ROTATE_RIGHT;
        break;

        case 9: // (1001) Both RIGHT and UP pressed
            g_rover_commands.left_speed = g_rover_speed.fast_speed_left;
            g_rover_commands.right_speed = 0;
            g_rover_commands.left_direction = FORWARD;
            g_rover_state.drive_state = DRIVE_STATE_TURN_RIGHT;
        break;

        case 10: // (1010) Both RIGHT and DOWN pressed
            g_rover_commands.left_speed = g_rover_speed.fast_speed_left;
            g_rover_commands.right_speed = 0;
            g_rover_commands.left_direction = REVERSE;
            g_rover_state.drive_state = DRIVE_STATE_REVERSE;
        break;

        case 11: // (1011) RIGHT, UP, and DOWN pressed
            g_rover_commands.left_speed = g_rover_speed.fast_speed_left;
            g_rover_commands.right_speed = g_rover_speed.fast_speed_right;
            g_rover_commands.left_direction = FORWARD;
            g_rover_commands.right_direction = REVERSE;
            g_rover_state.drive_state = DRIVE_STATE_ROTATE_RIGHT;
        break;

        case 12: // (1100) Both RIGHT and LEFT pressed
            g_rover_commands.left_speed = 0;
            g_rover_commands.right_speed = 0;
            g_rover_state.drive_state = DRIVE_STATE_STOPPED;
        break;

        case 13: // (1101) RIGHT, LEFT, and UP pressed
            g_rover_commands.left_speed = g_rover_speed.fast_speed_left;
            g_rover_commands.right_speed = g_rover_speed.fast_speed_right;
            g_rover_commands.left_direction = FORWARD;
            g_rover_commands.right_direction = FORWARD;
            g_rover_state.drive_state = DRIVE_STATE_FORWARD;
        break;

        case 14: // (1110) RIGHT, LEFT, and DOWN pressed
            g_rover_commands.left_speed = g_rover_speed.fast_speed_left;
            g_rover_commands.right_speed = g_rover_speed.fast_speed_right;
            g_rover_commands.left_direction = REVERSE;
            g_rover_commands.right_direction = REVERSE;
            g_rover_state.drive_state = DRIVE_STATE_REVERSE;
        break;

        case 15: // (1111) All four directions pressed
            g_rover_commands.left_speed = 0;
            g_rover_commands.right_speed = 0;
            g_rover_state.drive_state = DRIVE_STATE_STOPPED;
        break;

            /* This should never be reached */
        default:
            err = SSP_ERR_INVALID_ARGUMENT;
            g_rover_commands.left_speed = 0;
            g_rover_commands.right_speed = 0;
            g_rover_commands.left_direction = FORWARD;
            g_rover_commands.right_direction = FORWARD;
            g_rover_state.drive_state = DRIVE_STATE_STOPPED;
        break;
    }

    tx_semaphore_put(&g_motor_controller_semaphore);
    tx_semaphore_put(&g_led_driver_semaphore);
    tx_semaphore_put(&g_ultrasonic_sensor_semaphore);

    return err;
}

/*---------------------------------------------------------------------------*
 * Function: rover_btn_update
 *---------------------------------------------------------------------------*/
/** The rover button update function updates the struct variable which holds
 *  the button states.
 *
 *  @param [in] app_btn     Takes a button value
 *  @param [in] rover_btn_event Takes an event value
 *  @return                 Returns an SSP error
 *  @par Example Code:
 *  @code
 *  ssp_err_t err = SSP_SUCCESS;
 *  rover_btn_t btn = some_button;
 *  rover_btn_event_t = some_event;
 *  err = rover_btn_update(btn, event);
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
ssp_err_t rover_btn_update(rover_btn_t app_btn, rover_btn_event_t rover_btn_event) // released = 0, pressed = 1
{
    ssp_err_t err = SSP_SUCCESS;

    switch (app_btn)
    {
        case UP:
            g_app_buttons.up_pressed = rover_btn_event;
        break;

        case DOWN:
            g_app_buttons.down_pressed = rover_btn_event;
        break;

        case LEFT:
            g_app_buttons.left_pressed = rover_btn_event;
        break;

        case RIGHT:
            g_app_buttons.right_pressed = rover_btn_event;
        break;

        default:
        break;
    }

    return err;
}

/*---------------------------------------------------------------------------*
 * Function: rover_mode_get
 *---------------------------------------------------------------------------*/
/** The rover mode get function allows other threads to read the local
 *  variable used to hold the current rover mode.
 *
 *  @return                 Returns the rover mode
 *  @par Example Code:
 *  @code
 *  rover_mode_t mode = rover_mode_get();
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
rover_mode_t rover_mode_get(void)
{
    return g_rover_mode;
}

/*---------------------------------------------------------------------------*
 * Function: rover_mode_set
 *---------------------------------------------------------------------------*/
/** The rover mode set function allows other threads to write the local
 *  variable used to hold the current rover mode.
 *
 *  @par Example Code:
 *  @code
 *  rover_mode_t mode = some_value;
 *  rover_mode_set(mode);
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
void rover_mode_set(rover_mode_t mode)
{
    g_rover_mode = mode;
}
/*---------------------------------------------------------------------------*
 * Function: rover_state_get
 *---------------------------------------------------------------------------*/
/** The rover state get function allows other threads to read the local
 *  variable used to hold the current rover state.
 *
 *  @return                 Returns the rover mode
 *  @par Example Code:
 *  @code
 *  rover_mode_t mode = rover_mode_get();
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
void rover_state_get(rover_state_t * p_rover_state)
{
    *p_rover_state = g_rover_state;
}

/*---------------------------------------------------------------------------*
 * Function: rover_mode_set
 *---------------------------------------------------------------------------*/
/** The rover mode set function allows other threads to write the local
 *  variable used to hold the current rover mode.
 *
 *  @par Example Code:
 *  @code
 *  rover_mode_t mode = some_value;
 *  rover_mode_set(mode);
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
void rover_state_set(rover_state_t * p_rover_state)
{
    g_rover_state = *p_rover_state;
}

/*---------------------------------------------------------------------------*
 * Function: rover_cmd_get
 *---------------------------------------------------------------------------*/
/** The rover command get function allows other threads to read the local
 *  variable used to hold the current rover commands.
 *
 *  @return                 Returns the rover commands
 *  @par Example Code:
 *  @code
 *  rover_command_t cmd = rover_cmd_get();
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
void rover_cmd_get(rover_command_t * p_rover_command)
{
    *p_rover_command = g_rover_commands;
}

/*---------------------------------------------------------------------------*
 * Function: TODO:
 *---------------------------------------------------------------------------*/
/** TODO:
 *
 *  @return                 Returns the rover commands
 *  @par Example Code:
 *  @code
 *  rover_command_t cmd = rover_cmd_get();
 *  @endcode
 */
/*---------------------------------------------------------------------------*/
void rover_speed_get(rover_speed_t * p_rover_speed)
{
    *p_rover_speed = g_rover_speed;
}

/*-------------------------------------------------------------------------*
 * End of File:  rover.c
 *-------------------------------------------------------------------------*/
