#ifndef _LEDTEST_H_
#define _LEDTEST_H_
#define LED_DRIVER_NAME "/dev/periled"

int ledLibInit();
int ledOnOff(int ledNum, int onOff);
int ledStatus();
int ledLibExit();

#endif 
