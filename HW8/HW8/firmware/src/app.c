/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    
    __builtin_disable_interrupts();

    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);

    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;

    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;

    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;

    // do your TRIS and LAT commands here
    TRISAbits.TRISA4 = 0;
    LATAbits.LATA4 = 0; 
    
    __builtin_enable_interrupts();
    
    SPI1_init();
    LCD_init();
    initIMU();
    LCD_clearScreen(WHITE);
}


/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( appData.state )
    {
        /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;
       
        
            if (appInitialized)
            {
            
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {   
            start = _CP0_GET_COUNT();
            // read LSM6
            I2C_read_multiple(SLAVE_ADDR,0x20,raw,14);
            dataFormat(raw,final,7);
            // x direction (-y direction of LCD)
            if (final[4] < 0) {
                x = final[4]*CONV*-1;
                if (x_prev == 1) {
                    LCD_drawBar(0,64,128,BARWIDTH,WHITE); // clear bar on crossover
                }
                LCD_drawBar(0,64,64-x,BARWIDTH,WHITE);
                LCD_drawBar(64-x,64,x,BARWIDTH,RED);
                x_prev = 0;
            } 
            else {
                x = final[4]*CONV;
                if (x_prev == 0) {
                    LCD_drawBar(0,64,128,BARWIDTH,WHITE); // clear bar on crossover
                }
                LCD_drawBar(64,64,x,BARWIDTH,RED);
                LCD_drawBar(64+x,64,64-x,BARWIDTH,WHITE);
                x_prev = 1;
            }
            // y direction (-x direction of LCD)
            if (final[5] < 0) {
                y = final[5]*CONV*-1;
                if (y_prev == 1) {
                    LCD_drawBar(64,0,BARWIDTH,128,WHITE); // clear bar on crossover
                }
                LCD_drawBar(64,0,BARWIDTH,64-y,WHITE);
                LCD_drawBar(64,64-y,BARWIDTH,y,BLUE);
                y_prev = 0;
            } 
            else {
                y = final[5]*CONV;
                if (y_prev == 0) {
                    LCD_drawBar(64,0,BARWIDTH,128,WHITE); // clear bar on crossover
                }
                LCD_drawBar(64,64,BARWIDTH,y,BLUE);
                LCD_drawBar(64,64+y,BARWIDTH,64-y,WHITE);
                y_prev = 1;
            }


            while (_CP0_GET_COUNT() - start < 4800000) {;} // update screen at 5 Hz
            break;
        }

        /* TODO: implement your application state machine.*/
        

        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

 void dataFormat(unsigned char * raw, short * final, int length) {
    int i;
    unsigned char low, high;
    for (i = 0; i < length; i++) {
        low = raw[2*i];
        high = raw[2*i+1];
        final[i] = (high << 8) | low;
    }
}

/*******************************************************************************
 End of File
 */
