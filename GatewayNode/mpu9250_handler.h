#include <Wire.h>
#include <JY901.h>

int status;

void setupMPU9250()
{
    JY901.StartIIC();
}

String getMPU9250Values()
{
    // read the sensor
    String data = String((float)JY901.stcAcc.a[0] / 32768 * 16) + String(";") + String((float)JY901.stcAcc.a[1] / 32768 * 16) + String(";") + String((float)JY901.stcAcc.a[2] / 32768 * 16) +
                  String(";") + String((float)JY901.stcGyro.w[0] / 32768 * 2000) + String(";") + String((float)JY901.stcGyro.w[1] / 32768 * 2000) + String(";") + String((float)JY901.stcGyro.w[2] / 32768 * 2000) +
                  String(";") + String(JY901.stcMag.h[0]) + String(";") + String(JY901.stcMag.h[1]) + String(";") + String(JY901.stcMag.h[2]) +
                  String(";") + String((float)JY901.stcAngle.Angle[0] / 32768 * 180) + String(";") + String((float)JY901.stcAngle.Angle[1] / 32768 * 180) + String(";") + String((float)JY901.stcAngle.Angle[2] / 32768 * 180);
    delay(100);
    return data;
}