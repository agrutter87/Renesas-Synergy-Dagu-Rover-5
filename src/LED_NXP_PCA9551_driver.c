/*-------------------------------------------------------------------------*
 * File:  LED_NXP_PCA9551_driver.c
 *-------------------------------------------------------------------------*
 * Description:
 *      Device implementation of the LEDBank for the PCA9551
 *-------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------
 * uEZ(R) - Copyright (C) 2007-2015 Future Designs, Inc.
 *--------------------------------------------------------------------------
 * This file is part of the uEZ(R) distribution.  See the included
 * uEZ License.pdf or visit http://goo.gl/UDtTCR for details.
 *
 *    *===============================================================*
 *    |  Future Designs, Inc. can port uEZ(r) to your own hardware!  |
 *    |             We can get you up and running fast!               |
 *    |      See http://goo.gl/UDtTCR for more details.               |
 *    *===============================================================*
 *
 *-------------------------------------------------------------------------*/
#include "LED_NXP_PCA9551.h"

/*---------------------------------------------------------------------------*
 * Constants:
 *---------------------------------------------------------------------------*/
#define PCA9551_I2C_SPEED               400 // kHz

#define STATE_ON                        0
#define STATE_OFF                       1
#define STATE_BLINK0                    2
#define STATE_BLINK1                    3

/*---------------------------------------------------------------------------*
 * Types:
 *---------------------------------------------------------------------------*/

static const uint8_t lookupTable[4][4] =
{
{ (0 << 0), (1 << 0), (2 << 0), (3 << 0) },
  { (0 << 2), (1 << 2), (2 << 2), (3 << 2) },
  { (0 << 4), (1 << 4), (2 << 4), (3 << 4) },
  { (0 << 6), (1 << 6), (2 << 6), (3 << 6) }, };

static uint8_t IBuildLEDSelector(uint8_t *iLEDState)
{
    return lookupTable[0][iLEDState[0]] | lookupTable[1][iLEDState[1]] | lookupTable[2][iLEDState[2]]
            | lookupTable[3][iLEDState[3]];
}

static void IUnbuildLEDSelector(uint8_t *aLEDState, uint8_t bits)
{
    uint8_t i;

    for (i = 0; i < 4; i++)
    {
        *(aLEDState++) = (bits & 3);
        bits >>= 2;
    }
}

/*---------------------------------------------------------------------------*
 * Routine:  IReadLEDConfig
 *---------------------------------------------------------------------------*
 * Description:
 *      Read led configurate and store in settings
 * Inputs:
 *      T_LED_NXP_PCA9551_Workspace *p  -- Workspace
 * Outputs:
 *      ssp_err_t                   -- Error code
 *---------------------------------------------------------------------------*/
ssp_err_t IReadLEDConfig(T_LED_NXP_PCA9551_Workspace *p)
{
    uint8_t cmd[3];
    uint8_t settings[2];
    ssp_err_t error = SSP_SUCCESS;

    // Send out the 1 byte command
    cmd[0] = 0x15;

    error = p->iI2C->p_api->write (p->iI2C->p_ctrl, cmd, 1, 0, TX_WAIT_FOREVER);
    if (error)
    {
        return error;
    }

    // Interpret the settings
    IUnbuildLEDSelector (p->iLEDsState + 0, settings[0]);
    IUnbuildLEDSelector (p->iLEDsState + 4, settings[1]);

    return error;
}

/*---------------------------------------------------------------------------*
 * Routine:  IUpdateLEDs
 *---------------------------------------------------------------------------*
 * Description:
 *      Internal routine that does the actual I2C call
 * Inputs:
 *      void *aW                    -- Workspace
 * Outputs:
 *      ssp_err_t                   -- Error code
 *---------------------------------------------------------------------------*/
ssp_err_t IUpdateLEDs(T_LED_NXP_PCA9551_Workspace *p)
{
    uint8_t cmd[3];

    // Write out 2 bytes
    cmd[0] = 0x15;
    cmd[1] = IBuildLEDSelector (p->iLEDsState + 0);

    // (LED4 to LED7)
    cmd[2] = IBuildLEDSelector (p->iLEDsState + 4);

    ssp_err_t err = p->iI2C->p_api->write (p->iI2C->p_ctrl, cmd, 3, false, TX_WAIT_FOREVER);
    return err;
}

/*---------------------------------------------------------------------------*
 * Routine:  LED_NXP_PCA9551_InitializeWorkspace
 *---------------------------------------------------------------------------*
 * Description:
 *      Setup of this workspace for PCA9551.
 * Inputs:
 *      void *aW                    -- Workspace
 * Outputs:
 *      ssp_err_t                   -- Error code
 *---------------------------------------------------------------------------*/
ssp_err_t LED_NXP_PCA9551_InitializeWorkspace(void *aW)
{
    ssp_err_t error = SSP_SUCCESS;
    uint8_t i;

    T_LED_NXP_PCA9551_Workspace *p = (T_LED_NXP_PCA9551_Workspace *) aW;

    for (i = 0; i < 8; i++)
        p->iLEDsState[i] = STATE_OFF;

    return error;
}

/*---------------------------------------------------------------------------*
 * Routine:  LED_NXP_PCA9551_On
 *---------------------------------------------------------------------------*
 * Description:
 *      Turn on multiple LEDs
 * Inputs:
 *      void *aW                    -- Workspace
 *      T_uezTimeDate *aTimeDate    -- Time and date returned
 * Outputs:
 *      ssp_err_t                   -- Error code
 *---------------------------------------------------------------------------*/
ssp_err_t LED_NXP_PCA9551_On(void *aWorkspace, uint32_t aLEDs)
{
    T_LED_NXP_PCA9551_Workspace *p = (T_LED_NXP_PCA9551_Workspace *) aWorkspace;
    uint32_t bitMask;
    ssp_err_t error = SSP_SUCCESS;
    uint8_t i;

    // Allow only one transfer at a time
    tx_mutex_get (p->iSem, TX_WAIT_FOREVER);

    error = IReadLEDConfig (p);
    if (!error)
    {
        for (i = 0, bitMask = 1; i < 8; i++, bitMask <<= 1)
        {
            // Set the bits to on if bit is set
            if (aLEDs & bitMask)
                p->iLEDsState[i] = STATE_ON;
        }

        error = IUpdateLEDs (p);
    }

    tx_mutex_put (p->iSem);

    return error;
}

/*---------------------------------------------------------------------------*
 * Routine:  LED_NXP_PCA9551_Off
 *---------------------------------------------------------------------------*
 * Description:
 *      Turn off multiple LEDs
 * Inputs:
 *      void *aW                    -- Workspace
 *      T_uezTimeDate *aTimeDate    -- Time and date returned
 * Outputs:
 *      ssp_err_t                   -- Error code
 *---------------------------------------------------------------------------*/
ssp_err_t LED_NXP_PCA9551_Off(void *aWorkspace, uint32_t aLEDs)
{
    T_LED_NXP_PCA9551_Workspace *p = (T_LED_NXP_PCA9551_Workspace *) aWorkspace;
    uint8_t i;
    uint32_t bitMask;
    ssp_err_t error;

    // Allow only one transfer at a time
    tx_mutex_get (p->iSem, TX_WAIT_FOREVER);

    error = IReadLEDConfig (p);
    if (!error)
    {
        for (i = 0, bitMask = 1; i < 8; i++, bitMask <<= 1)
        {
            // Set the bits to on if bit is set
            if (aLEDs & bitMask)
                p->iLEDsState[i] = STATE_OFF;
        }

        // Update the LEDs state
        error = IUpdateLEDs (p);
    }

    tx_mutex_put (p->iSem);

    return error;
}

/*---------------------------------------------------------------------------*
 * Routine:  LED_NXP_PCA9551_Blink
 *---------------------------------------------------------------------------*
 * Description:
 *      Blink the given LEDs with the blink register.
 * Inputs:
 *      void *aW                    -- Workspace
 *      uint32_t aBlinkReg           -- Blink register used to control blink
 *      uint32_t aLEDs               -- Bit mask of LEDs to blink
 * Outputs:
 *      ssp_err_t                   -- Error code
 *---------------------------------------------------------------------------*/
ssp_err_t LED_NXP_PCA9551_Blink(void *aWorkspace, uint32_t aBlinkReg, uint32_t aLEDs)
{
    T_LED_NXP_PCA9551_Workspace *p = (T_LED_NXP_PCA9551_Workspace *) aWorkspace;
    uint8_t i;
    uint32_t bitMask;
    ssp_err_t error;
    uint8_t state = STATE_BLINK0;
    if (aBlinkReg)
        state = STATE_BLINK1;

    // Allow only one transfer at a time
    tx_mutex_get (p->iSem, TX_WAIT_FOREVER);

    error = IReadLEDConfig (p);
    if (!error)
    {
        for (i = 0, bitMask = 1; i < 8; i++, bitMask <<= 1)
        {
            // Set the bits to on if bit is set
            if (aLEDs & bitMask)
                p->iLEDsState[i] = state;
        }

        // Update the LEDs state
        error = IUpdateLEDs (p);
    }

    tx_mutex_put (p->iSem);

    return error;
}

/*---------------------------------------------------------------------------*
 * Routine:  LED_NXP_PCA9551_SetBlinkRate
 *---------------------------------------------------------------------------*
 * Description:
 *      Set the rate of a given blink register (as close as possible).
 * Inputs:
 *      void *aW                    -- Workspace
 *      uint32_t aBlinkReg           -- Blink register used to control blink
 *      uint32_t aPeriod             -- Period in ms
 *      uint8_t aDutyCycle           -- Duty cycle in 0 (0%) to 255 (100%)
 * Outputs:
 *      ssp_err_t                   -- Error code
 *---------------------------------------------------------------------------*/
ssp_err_t LED_NXP_PCA9551_SetBlinkRate(void *aWorkspace, uint32_t aBlinkReg, uint32_t aPeriod, uint8_t aDutyCycle)
{
    T_LED_NXP_PCA9551_Workspace *p = (T_LED_NXP_PCA9551_Workspace *) aWorkspace;
    uint8_t cmd[3] =
    { 0x11, 0x00, 0x00 };
    uint32_t v;
    ssp_err_t error;

    if (aBlinkReg)
        cmd[0] = 0x13;

    // Determine what the value is for the hertz (0-255)
    // Formula:  period = (1+v)/38 sec;
    //  or 38/(1+v) Hz
//    if (aPeriod > 1000)
//        v = 0;
//    else
//        v = ((aPeriod * 38) / 1000) - 1;
//    if (v > 255)
//        v = 255;
//    cmd[1] = v;
    cmd[1] = aPeriod;

    // The duty cycle is (256-aDutyCycle)/256 percent
    // We just need to flip it around so 0 is 255 and 255 is 0.
    cmd[2] = 255 - aDutyCycle;

    // Allow only one transfer at a time
    tx_mutex_get (p->iSem, TX_WAIT_FOREVER);

    error = p->iI2C->p_api->write (p->iI2C->p_ctrl, cmd, 3, 0, TX_WAIT_FOREVER);

    tx_mutex_put(p->iSem);

    return error;
}

/*-------------------------------------------------------------------------*
 * End of File:  LED_NXP_PCA9551_driver.c
 *-------------------------------------------------------------------------*/
