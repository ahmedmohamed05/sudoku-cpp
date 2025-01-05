#pragma once
#include <ctime>
#include <iostream>

class Timer {

private:
  clock_t _startTime, _endTime;

public:
  void start() { _startTime = clock(); }
  void stop() { _endTime = clock(); }

  float getSeconds() const {
    return float(_endTime - _startTime) / CLOCKS_PER_SEC;
  }

  float getMinutes() const { return getSeconds() / 60; }

  float getHours() const { return getMinutes() / 60; }
};