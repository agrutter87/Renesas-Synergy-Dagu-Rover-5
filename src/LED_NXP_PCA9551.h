/*-------------------------------------------------------------------------*
 * File:  LED_NXP_PCA9551.h
 *-------------------------------------------------------------------------*
 * Description:
 *
 *-------------------------------------------------------------------------*/
#ifndef LED_NXP_PCA9551_H_
#define LED_NXP_PCA9551_H_

/*--------------------------------------------------------------------------
* uEZ(r) - Copyright (C) 2007-2015 Future Designs, Inc.
*--------------------------------------------------------------------------
* This file is part of the uEZ(r) distribution.
*
* uEZ(r) is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* uEZ(r) is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with uEZ(r); if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
*
* A special exception to the GPL can be applied should you wish to
* distribute a combined work that includes uEZ(r), without being obliged
* to provide the source code for any proprietary components.  See the
* licensing section of http://goo.gl/UDtTCR for full details of how
* and when the exception can be applied.
*
*    *===============================================================*
*    |  Future Designs, Inc. can port uEZ(r) to your own hardware!   |
*    |             We can get you up and running fast!               |
*    |      See http://goo.gl/UDtTCR for more details.               |
*    *===============================================================*
*
*-------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------*
 * Includes:
 *-------------------------------------------------------------------------*/
#include "led_driver_thread.h"

#ifdef __cplusplus
extern "C" {
#endif

/*-------------------------------------------------------------------------*
 * Constants:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Types:
 *-------------------------------------------------------------------------*/
typedef struct
{
    TX_MUTEX * iSem;
    sf_i2c_instance_t * iI2C;
    uint8_t iLEDsState[8];
} T_LED_NXP_PCA9551_Workspace;

/*-------------------------------------------------------------------------*
 * Globals:
 *-------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*
 * Prototypes:
 *-------------------------------------------------------------------------*/
ssp_err_t IReadLEDConfig(T_LED_NXP_PCA9551_Workspace *p);
ssp_err_t IUpdateLEDs(T_LED_NXP_PCA9551_Workspace *p);
ssp_err_t LED_NXP_PCA9551_InitializeWorkspace(void *aW);
ssp_err_t LED_NXP_PCA9551_On(void *aWorkspace, uint32_t aLEDs);
ssp_err_t LED_NXP_PCA9551_Off(void *aWorkspace, uint32_t aLEDs);
ssp_err_t LED_NXP_PCA9551_Blink(void *aWorkspace, uint32_t aBlinkReg, uint32_t aLEDs);
ssp_err_t LED_NXP_PCA9551_SetBlinkRate(void *aWorkspace, uint32_t aBlinkReg, uint32_t aPeriod, uint8_t aDutyCycle);

#ifdef __cplusplus
}
#endif

#endif // LED_NXP_PCA9551_H_
/*-------------------------------------------------------------------------*
 * End of File:  LED_NXP_PCA9551.h
 *-------------------------------------------------------------------------*/
