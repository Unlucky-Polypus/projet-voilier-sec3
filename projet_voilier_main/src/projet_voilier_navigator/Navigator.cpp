#include "Arduino.h"
#include "Navigator.h"

Navigator::Navigator(Point buoy) {
  this->buoy = buoy;
}

void Navigator::setBuoy(Point buoy) {
  this->buoy = buoy;
}


// Returns the input of the actuators in percentage from the position of the boat, the wind direction and the actual course
CommandOutput Navigator::navigate(Point boat, float wind_angle, float course) {
  this->position = boat;
  this->wind_angle = wind_angle;
  this->course = course;


  float azimuth = compute_azimuth();
  CommandOutput commands;

  if (check_straight_line(azimuth)) {
    commands.safran = this->adjust_traj(azimuth);
  } else {
    commands.safran = this->adjust_traj(choose_side_tack(azimuth));
  }

  commands.sails = this->adjust_sail(azimuth);

  return commands;
}

// Compute the azimuth (angle between the North, the boat and the buoy) from their GPS coordinates
// boat : GPS coordinates of the boat
// buoy : GPS coordinates of the buoy
// azimuth : Azimuth in degrees in the range -180 / 180
float Navigator::compute_azimuth() {
  float lat_boat = radians(this->position.lat);
  float long_boat = radians(this->position.lon);
  float lat_buoy = radians(this->buoy.lat);
  float long_buoy = radians(this->buoy.lon);

  float delta_long = long_buoy - long_boat;
  float y = sin(delta_long) * cos(lat_buoy);
  float x = cos(lat_boat) * sin(lat_buoy) - sin(lat_boat) * cos(lat_buoy) * cos(delta_long);
  float azimuth = atan2(y,x);
  azimuth = azimuth * 180 / M_PI;

  return azimuth;
}


// Check wether straight line is possible (wind angle < MIN_WIND_ANGLE)
bool Navigator::check_straight_line(float azimuth) {
  float wind_forecast = this->compute_wind_projection(azimuth);
  if (abs(azimuth - wind_forecast) < MIN_WIND_ANGLE) return false;
  return true;
}
    

// Compute projected  wind angle on a given course
// boat : Boat GPS position and course
// wind_angle : Wind direction in the boat referential
// new_course : New course to be computed
// projected _wind : Wind direction in the referential of the boat projected onto the new course
float Navigator::compute_wind_projection(float new_course) {
  float course_diff = recalibrate_angle_180(this->course - new_course);
  float projected_wind = recalibrate_angle_180(this->wind_angle + course_diff);
  return projected_wind;
}


// Choos on which side we need to go if we are up the wind
float Navigator::choose_side_tack(float azimuth) {
  float wind = convert_180_to_360(this->wind_angle);
  azimuth = convert_180_to_360(azimuth);
  float angle;

  if (azimuth < 180) {
    if (wind > azimuth && wind < azimuth + 180) {
      angle = recalibrate_angle_360(wind - MIN_WIND_ANGLE);
    } else {
      angle = recalibrate_angle_360(wind + MIN_WIND_ANGLE);
    }
  } else {
    if (wind < azimuth && wind > azimuth - 180) {
      angle = recalibrate_angle_360(wind + MIN_WIND_ANGLE);
    } else {
      angle = recalibrate_angle_360(wind - MIN_WIND_ANGLE);
    }
  }  
  return angle;
}


// La sortie est un pourcentage de la position du moteur (PWM)
// On suppose que le rapport entre la position du moteur et celle de la voile est linéaire
// On a un angle de voile pour chaque allure (au près, de travers, vent de face et vent arrière)
float Navigator::adjust_sail(float azimuth) {
  float wind_angle = this->wind_angle;
  float abs_wind_angle = abs(wind_angle);
  float sail_angle;
  // Computing sail angle in the boat coordinate system
  // Vent arrière
  if (abs_wind_angle <= 180 && abs_wind_angle > 135) {
    if (wind_angle >= 0) {
      sail_angle = recalibrate_angle_180(wind_angle + 90);
    } else {
      sail_angle = recalibrate_angle_180(wind_angle - 90);
    }
  // Vent de travers
  } else if(abs_wind_angle > 80) {
    if (wind_angle >= 0) {
      sail_angle = 140;
    } else {
      sail_angle = -140;
    }
  // Au près
  } else if(abs_wind_angle > 35) {
    if (wind_angle >= 0) {
      sail_angle = -165;
    } else {
      sail_angle = 165;
    }
  // Vent de face
  } else {
    if (wind_angle >= 0) {
      sail_angle = -90;
    } else {
      sail_angle = 90;
    }
  }
  // On retourne le pourcentage de la PWM associé à l'angle de la voile voilu
  return abs(recalibrate_angle_180(sail_angle + 180)) * 100 / 90;
}


// Sortie en pourcentage de la position du moteur (PWM)
// La commande est proportionelle à la différecence de trajectoire
float Navigator::adjust_traj(float new_course) {
  float course_diff = new_course - this->course;
  return (course_diff + 180) * 100 / 360;
}




