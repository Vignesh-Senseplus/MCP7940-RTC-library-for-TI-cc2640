/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/System.h>

#include "stdio.h"
#include "string.h"
#include "math.h"
#include "inttypes.h"

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/PIN.h>
#include <ti/drivers/I2C.h>
#include <ti/drivers/UART.h>
#include <ti/display/Display.h>

/* Example/Board Header files */
#include "Board.h"
#include "RTC.h"

/******************************************************************************************************************/

#define TASKSTACKSIZE 1024




uint8_t      timestamp[7];

//#define BoostEn IOID_9
/* Global memory storage for a PIN_Config table */


/*
 * Application LED pin configuration table:
 * - All LEDs board LEDs are off.
 */
Display_Handle dispHandle = NULL;

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];
static PIN_State ledPinState;
static PIN_Handle ledPinHandle;
PIN_Config ledPinTable[] = {
                            Board_PIN_LED0 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
                            Board_PIN_LED1 | PIN_GPIO_OUTPUT_EN | PIN_GPIO_LOW | PIN_PUSHPULL | PIN_DRVSTR_MAX,
                            SE | PIN_GPIO_OUTPUT_EN | PIN_GPIO_HIGH | PIN_PUSHPULL | PIN_DRVSTR_MAX,
                            PIN_TERMINATE
};
UART_Handle uart;
UART_Params uartParams;
I2C_Handle i2c;
I2C_Params i2cParams;
I2C_Transaction i2cTransaction;


uint8_t convertFromBcd(uint8_t byteBCD);
uint8_t convertToBcd(uint8_t byteDecimal);
void init_MCP7940();


Void taskFxn(UArg arg0, UArg arg1)
{
    uint8_t data[7]={0,07,14,6,27,03,15};



    if(CheckI2c() == true)
     {
         System_printf(" i2c Started  \n");
           System_flush();

     }
     else
     {
         System_printf(" i2c failed  \n");
              System_flush();
     }

    WriteRtc(data);

     if(isRTCrunning() == false)
     {
         StartRTC();

    }

   while(1)
   {
       memcpy(data,ReadRtc(),sizeof(ReadRtc()));
       System_printf("Values %d:%d:%d...%d/%d/%d  %d \n",data[2],data[1],data[0],data[4],data[5],data[6],data[3]);
                     System_flush();

   }


}













/*
 * ======== main ========
 */
int main(void)
{

    Task_Params taskParams;
    uint32_t st;

    /* Call board init functions */
    // Board_initGeneral();
    //Board_init();

    PIN_init(BoardGpioInitTable);
   // I2C_init();
    UART_init();



    /* Create a UART with data processing off. */
    UART_Params_init(&uartParams);
    uartParams.writeDataMode = UART_DATA_BINARY;
    uartParams.readDataMode = UART_DATA_BINARY;
    uartParams.readReturnMode = UART_RETURN_FULL;
    uartParams.readEcho = UART_ECHO_OFF;
    uartParams.baudRate = 115200;
    uart = UART_open(Board_UART0, &uartParams);

    if (uart == NULL) {
        //System_abort("Error opening the UART");
    }

    /* Construct tmp007 Task thread */
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)taskFxn, &taskParams, NULL);

    /* Open LED pins */
    ledPinHandle = PIN_open(&ledPinState, ledPinTable);
    if(!ledPinHandle) {
        System_abort("Error initializing board LED pins\n");
    }




    // PIN_setOutputValue(ledPinHandle, Board_LED2, 0);
    //PIN_setPortOutputValue(ledPinHandle, 1 );
    /* Start BIOS */
    BIOS_start();

    return (0);
}
