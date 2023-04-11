#include <Wire.h>
#include <Adafruit_ADS1X15.h>
Adafruit_ADS1115 ads;

int adc0, adc1, adc2, adc3;             // ADC Readings
int X1angle, Y1angle, X2angle, Y2angle; // Joystick Angle Variables

void setupADS1115()
{
    ads.begin();
}
uint16_t getADS1115Values(int type)
{
    adc0 = ads.readADC_SingleEnded(0);
    adc1 = ads.readADC_SingleEnded(1);
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