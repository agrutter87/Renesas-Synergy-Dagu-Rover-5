/*-------------------------------------------------------------------------*
 * File:  rover.h
 *-------------------------------------------------------------------------*
 * Description:
 */
/**
 *	@file 	rover.h
 *  @brief 	TODO: Short description
 *
 *	TODO: Description of file
 */
/*-------------------------------------------------------------------------*/
#ifndef ROVER_H_
#define ROVER_H_

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include "bluetooth_thread.h"
#include "rover_thread.h"
#include "motor_controller.h"
#include "datalogger.h"
#include "led_driver.h"
#include "ultrasonic_sensor.h"
#include "ultrasonic_sensor_thread.h"
/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/
#define PRESSED_FORWARD_CHAR 'D'
#define RELEASED_FORWARD_CHAR 'd'
#define PRESSED_REVERSE_CHAR 'E'
#define RELEASED_REVERSE_CHAR 'e'
#define PRESSED_RIGHT_CHAR 'B'
#define RELEASED_RIGHT_CHAR 'b'
#define PRESSED_LEFT_CHAR 'C'
#define RELEASED_LEFT_CHAR 'c'
#define CHANGE_MODE_CHAR 'a'

#define FAST_SPEED 100000 // %PWM*1000
#define SLOW_SPEED 50000

/*-------------------------------------------------------------------------*
 * Types:
 *-------------------------------------------------------------------------*/
typedef enum
{
    MANUAL = 0, PLAYBACK
} rover_mode_t;

typedef enum
{
    DRIVE_STATE_STOPPED = 0,
    DRIVE_STATE_FORWARD,
    DRIVE_STATE_REVERSE,
    DRIVE_STATE_TURN_LEFT,
    DRIVE_STATE_TURN_RIGHT,
    DRIVE_STATE_ROTATE_LEFT,
    DRIVE_STATE_ROTATE_RIGHT
} rover_drive_state_t;

typedef enum
{
    SWITCH_STATE_OFF = 0,
    SWITCH_STATE_PARKING,
    SWITCH_STATE_ON,
} headlight_switch_state_t;

typedef struct
{
    headlight_switch_state_t headlight_switch_state;
    bool bright_switch_state;
    bool hazard_switch_state;
} rover_light_state_t;

typedef struct
{
    rover_drive_state_t drive_state;
    rover_light_state_t light_state;
} rover_state_t;

typedef enum
{
    UP = 0, DOWN, LEFT, RIGHT
} rover_btn_t;

typedef enum
{
    RELEASED = 0, PRESSED
} rover_btn_event_t;

typedef struct
{
    union
    {
        uint8_t buttons_pressed;
        struct
        {
            uint8_t up_pressed :1;
            uint8_t down_pressed :1;
            uint8_t left_pressed :1;
            uint8_t right_pressed :1;
            uint8_t reserved :4;
        };
    };
} rover_app_button_t;

typedef enum
{
    REVERSE = 0,
    FORWARD = 1
} rover_direction_t;

typedef struct
{
    timer_size_t left_speed;
    timer_size_t right_speed;
    rover_direction_t left_direction;
    rover_direction_t right_direction;
} rover_command_t;

typedef struct
{
    timer_size_t fast_speed_right;
    timer_size_t slow_speed_right;
    timer_size_t fast_speed_left;
    timer_size_t slow_speed_left;
} rover_speed_t;



/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/
ssp_err_t rover_cmd_update(void);
ssp_err_t rover_btn_update(rover_btn_t rover_btn, rover_btn_event_t rover_btn_event);
rover_mode_t rover_mode_get(void);
void rover_mode_set(rover_mode_t mode);
void rover_state_get(rover_state_t *);
void rover_state_set(rover_state_t *);

void rover_cmd_get(rover_command_t * p_rover_command);
void rover_speed_set(rover_speed_t speed);

#endif // ROVER_H_
/*-------------------------------------------------------------------------*
 * End of File:  rover.h
 *-------------------------------------------------------------------------*/
