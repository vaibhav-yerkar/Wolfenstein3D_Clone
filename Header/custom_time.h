#ifndef CUSTOMTIME_H
#define CUSTOMTIME_H

#include <chrono>

class Time
{
public:
  static const long SECOND = 1000000000L;

  static double getTime();
  static double getDelta();

  static void setDelta(double value);

protected:
private:
  static double m_delta;
  static std::chrono::high_resolution_clock::time_point m_epoch;
};

#endif
