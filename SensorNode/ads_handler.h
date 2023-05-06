#include "ADS1X15.h"
ADS1115 ADS(0x48);

void setupADS1115()
{
    ADS.begin();
}
uint16_t getADS1015Values(int type)
{
    ADS.setGain(0);
    int16_t adc0 = ADS.readADC(0);
    int16_t adc1 = ADS.readADC(1);
    float f = ADS.toVoltage(1); // voltage factor
    float v1=f*adc0;
    float v2=f*adc1;

    if (type == A_IN1)
    {
        // X1angle = map(adc2, 15, 23564, 0, 180);
        return adc0;
    }
    else if (type == A_IN2)
    {
        // Y1angle = map(adc3, 1818, 23564, 0, 180);
        return adc1;
    }
}