#ifndef Navigator_h
#define Navigator_h

#include "Arduino.h"
#include "utils.h"

#define MIN_WIND_ANGLE 35.0
#define MAX_SAFRAN_ANGLE = 30

struct Point {
  float lat;
  float lon;
};

struct Boat {
  Point pos;
  float course;
};

struct CommandOutput {
  float safran;
  float sails;
};

class Navigator {
  private:
    Point buoy;
    Point position;
    float course;
    float wind_angle;

    // Nav functions
    float compute_azimuth();
    bool check_straight_line(float azimuth);
    float compute_wind_projection(float new_course);
    float choose_side_tack(float azimuth);
    // Commands functions
    float adjust_traj(float azimuth);
    float adjust_sail(float azimuth);

  public:
    Navigator(Point buoy);
    void setBuoy(Point buoy);
    CommandOutput navigate(Point boat, float wind_angle, float course);
    bool arrived(float precision);
};

#endif
