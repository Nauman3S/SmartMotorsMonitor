#include "MPU9250.h"

// an MPU9250 object with the MPU-9250 sensor on I2C bus 0 with address 0x68
MPU9250 IMU(Wire, 0x68);
int status;

void setupMPU9250()
{
    delay(500);
    // start communication with IMU
    status = IMU.begin();
    if (status < 0)
    {
        Serial.println("IMU initialization unsuccessful");
        Serial.println("Check IMU wiring or try cycling power");
        Serial.print("Status: ");
        Serial.println(status);
        while (1)
        {
        }
    }
}

String getMPU9250Values()
{
    // read the sensor
    IMU.readSensor();
    String data = String(IMU.getAccelX_mss()) + String(";") + String(IMU.getAccelY_mss()) + String(";") + String(IMU.getAccelZ_mss()) +
                  String(IMU.getGyroX_rads()) + String(";") + String(IMU.getGyroY_rads()) + String(";") + String(IMU.getGyroZ_rads()) +
                  String(IMU.getMagX_uT()) + String(";") + String(IMU.getMagY_uT()) + String(";") + String(IMU.getMagZ_uT()) +
                  String(";") + String(IMU.getTemperature_C());
    // display the data
    // Serial.print(IMU.getAccelX_mss(), 6);
    // Serial.print("\t");
    // Serial.print(IMU.getAccelY_mss(), 6);
    // Serial.print("\t");
    // Serial.print(IMU.getAccelZ_mss(), 6);
    // Serial.print("\t");
    // Serial.print(IMU.getGyroX_rads(), 6);
    // Serial.print("\t");
    // Serial.print(IMU.getGyroY_rads(), 6);
    // Serial.print("\t");
    // Serial.print(IMU.getGyroZ_rads(), 6);
    // Serial.print("\t");
    // Serial.print(IMU.getMagX_uT(), 6);
    // Serial.print("\t");
    // Serial.print(IMU.getMagY_uT(), 6);
    // Serial.print("\t");
    // Serial.print(IMU.getMagZ_uT(), 6);
    // Serial.print("\t");
    // Serial.println(IMU.getTemperature_C(), 6);
    delay(100);
    return data;
}