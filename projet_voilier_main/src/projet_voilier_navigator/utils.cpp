#include "utils.h"

// Recalibrate an angle that goes over 180° or under -180°
float recalibrate_angle_180(float angle) {
  angle = fmodf(angle, 360);
  if (angle > 180.0) {
    angle -= 360.0;
  } else if (angle < -180.0) {
    angle += 360.0;
  }
  return angle;
}

float recalibrate_angle_360(float angle) {
  angle = fmodf(angle, 360.0);
  if (angle < 0) {
    return 360 + angle;
  }
  return angle;
  // return fmodf(angle, 360.0);
}


float convert_180_to_360(float angle) {
  if (angle < 0) {
    return 360 + angle;
  }
  return angle;
}