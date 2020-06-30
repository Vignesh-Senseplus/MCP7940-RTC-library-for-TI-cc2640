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



/* Example/Board Header files */
#include "Board.h"
#include "RTC.h"

/******************************************************************************************************************/

#define TASKSTACKSIZE 1024

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

uint8_t convertFromBcd(uint8_t byteBCD);
uint8_t convertToBcd(uint8_t byteDecimal);
void init_MCP7940();


Void taskFxn(UArg arg0, UArg arg1)
{
    uint8_t data[7]={0,07,14,6,27,03,15};//datas sent to rtc..0sec,07min,14hrs,6dy of the week,27day,03month,15year.



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
    PIN_init(BoardGpioInitTable);
   
    /* Construct tmp007 Task thread */
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    Task_construct(&task0Struct, (Task_FuncPtr)taskFxn, &taskParams, NULL);

   


  
    /* Start BIOS */
    BIOS_start();

    return (0);
}
