/*-------------------------------------------------------------------------*
 * File:  bluetooth.h
 *-------------------------------------------------------------------------*
 * Description:
 */
/**
 *	@file 	bluetooth.h
 *  @brief 	TODO: Short description
 *
 *	TODO: Description of file
 */
/*-------------------------------------------------------------------------*/
#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include "bluetooth_thread.h"
#include "rover.h"
#include "datalogger.h"

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/
#define BLUETOOTH_ON_PMOD1
//#define BLUETOOTH_ON_PMOD2

#ifdef BLUETOOTH_ON_PMOD1
#define BLUETOOTH_POWER_PIN     IOPORT_PORT_08_PIN_06
#define BLUETOOTH_STATUS_PIN    IOPORT_PORT_07_PIN_06
#endif
#ifdef BLUETOOTH_ON_PMOD2
#define BLUETOOTH_POWER_PIN     IOPORT_PORT_08_PIN_07
#define BLUETOOTH_STATUS_PIN    IOPORT_PORT_05_PIN_06
#endif
/*-------------------------------------------------------------------------*
 * Types:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/
ssp_err_t bt_reset(void);
ssp_err_t bt_connect(void);
ssp_err_t bt_read(void);
ssp_err_t bt_cmd_eval(void);
uint8_t bt_cmd_get(void);
void bt_cmd_set(uint8_t cmd);
void bluetooth_thread_entry(void);

#endif // BLUETOOTH_H_
/*-------------------------------------------------------------------------*
 * End of File:  bluetooth.h
 *-------------------------------------------------------------------------*/
