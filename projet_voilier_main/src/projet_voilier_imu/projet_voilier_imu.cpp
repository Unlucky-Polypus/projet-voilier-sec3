#include "Wire.h"
#include "projet_voilier_imu.h"

void Imu_Voilier::read_calibration_status(){
  Wire.beginTransmission(IMU_ADDR); // Address of the device
  Wire.write(CALIBRATION_REGISTER); // Register to read from
  Wire.endTransmission(false); // Send repeated start condition
  Wire.requestFrom(IMU_ADDR, 1); // Request 6 bytes from the device
  while (Wire.available() < 1); // Wait until all 6 bytes are available
  uint8_t calibration_status = Wire.read() << 1 | Wire.read();  // Combine two bytes into a 16-bit integer for X
  Serial.println(calibration_status);
  this->calibrated = (calibration_status == 255);
}

Imu_Voilier::Imu_Voilier(){
  this->calibrated = false;
  this->angle_value = 0;
  while (!calibrated) {
    read_calibration_status();
    delay(100);
  }
}

bool Imu_Voilier::imu_update_data(){
  Wire.beginTransmission(IMU_ADDR); // Address of the device
  Wire.write(REGISTER_VALUE_ANGLE); // Register to read from
  Wire.endTransmission(false); // Send repeated start condition
  Wire.requestFrom(IMU_ADDR, 2); // Request 6 bytes from the device
  while (Wire.available() < 2); // Wait until all 6 bytes are available
  this->angle_value = Wire.read() << 8 | Wire.read();  // Combine two bytes into a 16-bit integer for X
  return (this->angle_value >= 0 && this->angle_value <= 3599);
}

int16_t Imu_Voilier::imu_get_data(){
  return this->angle_value;
}

int16_t Imu_Voilier::imu_get_data_with_negative(){
  return this->angle_value - 1800;
}

void Imu_Voilier::imu_display_data(){
  Serial.print("Angle Value = ");
  Serial.print(this->angle_value);
  Serial.print("\n");
}