/*
 * RTC.c
 *
 *  Created on: Jun 26, 2020
 *      Author: senseplus
 */

#include <ti/drivers/I2C.h>
#include "RTC.h"
#include "Board.h"



uint8_t      timestamp[7];
static I2C_Handle i2c;
static I2C_Params i2cParams;
static I2C_Transaction i2cTransaction;

uint8_t convertFromBcd(uint8_t byteBCD);
uint8_t convertToBcd(uint8_t byteDecimal);

bool CheckI2c();
bool isRTCrunning();
void StartRTC();
uint8_t* ReadRtc();
void WriteRtc(uint8_t Data[8]);

bool CheckI2c()
{
    I2C_init();
    I2C_Params_init(&i2cParams);
    i2cParams.bitRate = I2C_400kHz;
    i2c = I2C_open(Board_I2C_TMP, &i2cParams);
    if (i2c == NULL)
    {
        return false;
    }
    else
        return true;
}

bool isRTCrunning()
{
    uint8_t RxBuffer[1];
    uint8_t TxBuffer[2];
    i2cTransaction.slaveAddress = MCP7940_I2C;
    i2cTransaction.writeBuf=REG_RTCSEC;
    i2cTransaction.writeCount=1;
    i2cTransaction.readBuf=NULL;
    i2cTransaction.readCount=0;
    I2C_transfer(i2c, &i2cTransaction);
    i2cTransaction.slaveAddress = MCP7940_I2C;
    i2cTransaction.readBuf =RxBuffer ;
    i2cTransaction.readCount = 1;
    i2cTransaction.writeBuf = NULL;
    i2cTransaction.writeCount = 0;
    I2C_transfer(i2c, &i2cTransaction);
    //System_printf(" Seconds Register Initial Value %d  \n",   RxBuffer[0]);
    //System_flush();
    if(RxBuffer[0] >= 80)
        return true;
    else
        return false;

}


uint8_t convertToBcd(uint8_t byteDecimal) {

    return ((byteDecimal / 10) << 4 | (byteDecimal % 10));

}

uint8_t convertFromBcd(uint8_t byteBCD)
{

    uint8_t byteMSB = 0;
    uint8_t byteLSB = 0;

    byteMSB      = (byteBCD & 0b11110000) >> 4;
    byteLSB      = (byteBCD & 0b00001111);

    return       ((byteMSB * 10) + byteLSB);
}

void StartRTC()
{
    uint8_t TxBuffer[2];
    TxBuffer[0]=REG_RTCSEC;
    TxBuffer[1]=convertToBcd(80);

    i2cTransaction.slaveAddress = MCP7940_I2C;
    i2cTransaction.writeBuf=TxBuffer;
    i2cTransaction.writeCount=2;
    i2cTransaction.readBuf=NULL;
    i2cTransaction.readCount=0;
    I2C_transfer(i2c, &i2cTransaction);

}

void WriteRtc(uint8_t Data[7])
{
    uint8_t TxBuffer[8];


    TxBuffer[0] = REG_RTCSEC;
    TxBuffer[1]  = convertToBcd(Data[0]);    // SECONDS
    TxBuffer[2]  = convertToBcd(Data[1]);    // MINUTES
    TxBuffer[3]  = convertToBcd(Data[2]);    // HOURS
    TxBuffer[4]  = convertToBcd(Data[3]);    // DAY OF WEEK (arbitrary value 1 - 7)
    TxBuffer[5]  = convertToBcd(Data[4]);    // DAY
    TxBuffer[6]  = convertToBcd(Data[5]);    // MONTH
    TxBuffer[7]  = convertToBcd(Data[6]);    // YEAR
    i2cTransaction.slaveAddress = MCP7940_I2C;
    i2cTransaction.writeBuf=TxBuffer;
    i2cTransaction.writeCount=8;
    i2cTransaction.readBuf=NULL;
    i2cTransaction.readCount=0;
    I2C_transfer(i2c, &i2cTransaction);
}

 uint8_t* ReadRtc()
{
   uint8_t timestamp[8];
    uint8_t RxBuffer[7];
    i2cTransaction.slaveAddress = MCP7940_I2C;
    i2cTransaction.writeBuf=REG_RTCSEC;
    i2cTransaction.writeCount=1;
    i2cTransaction.readBuf=NULL;
    i2cTransaction.readCount=0;
    I2C_transfer(i2c, &i2cTransaction);
    i2cTransaction.slaveAddress = MCP7940_I2C;
    i2cTransaction.readBuf =RxBuffer ;
    i2cTransaction.readCount = 7;
    i2cTransaction.writeBuf = NULL;
    i2cTransaction.writeCount = 0;
    I2C_transfer(i2c, &i2cTransaction);
    RxBuffer[0]=RxBuffer[0]&0b01111111;
    timestamp[0] = (convertFromBcd(RxBuffer[0]) & 0x7F);
    timestamp[1] = (convertFromBcd(RxBuffer[1]) & 0x7F);
    timestamp[2] = convertFromBcd(RxBuffer[2]) & 0x3F;
    timestamp[3] = convertFromBcd(RxBuffer[3]) & 0x07;
    timestamp[4] = convertFromBcd(RxBuffer[4]);
    timestamp[5] = convertFromBcd(RxBuffer[5]);
    timestamp[6] = convertFromBcd(RxBuffer[6]);
    timestamp[7] = convertFromBcd(RxBuffer[7]);
    return timestamp;
}
