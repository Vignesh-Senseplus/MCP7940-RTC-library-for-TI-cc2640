/*
 * RTC.h
 *
 *  Created on: Jun 26, 2020
 *      Author: senseplus
 */

#ifndef APPLICATION_RTC_H_
#define APPLICATION_RTC_H_

#define MCP7940_I2C   0x6F;  // I2C Address for the RTC
#define REG_RTCSEC    0x00;  // Register Address: Time Second
#define REG_RTCMIN    0x01;  // Register Address: Time Minute
#define REG_RTCHOUR   0x02;  // Register Address: Time Hour
#define REG_RTCWKDAY  0x03;  // Register Address: Date Day of Week
#define REG_RTCDATE   0x04;  // Register Address: Date Day
#define REG_RTCMTH    0x05;  // Register Address: Date Month
#define REG_RTCYEAR   0x06;  // Register Address: Date Year




extern bool CheckI2c();
extern bool isRTCrunning();
extern uint8_t* ReadRtc();
extern void WriteRtc(uint8_t Data[8]);
extern void StartRTC();


#endif /* APPLICATION_RTC_H_ */
