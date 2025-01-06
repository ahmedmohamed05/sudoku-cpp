#pragma once
#include <chrono>
#include <iostream>

class Timer {
private:
  // Using std::chrono to track time points
  std::chrono::time_point<std::chrono::steady_clock> _startTime;
  std::chrono::time_point<std::chrono::steady_clock> _endTime;
  bool _isRunning = false; // Flag to indicate if the timer is running

public:
  // Start the timer
  void start() {
    _startTime = std::chrono::steady_clock::now();
    _isRunning = true;
  }

  // Stop the timer
  void stop() {
    _endTime = std::chrono::steady_clock::now();
    _isRunning = false;
  }

  // Get elapsed time in seconds
  double getSeconds() const {
    auto endTime = _isRunning ? std::chrono::steady_clock::now() : _endTime;
    return std::chrono::duration<double>(endTime - _startTime).count();
  }

  // Get elapsed time in minutes
  double getMinutes() const { return getSeconds() / 60.0; }

  // Get elapsed time in hours
  double getHours() const { return getSeconds() / 3600.0; }

  int getTotalSeconds() const { return static_cast<int>(getSeconds()); }

  // h for human-readable
  int getHoursH() const { return getTotalSeconds() / 3600; }

  // h for human-readable
  int getMinutesH() const { return (getTotalSeconds() % 3600) / 60; }

  // h for human-readable
  int getSecondsH() const { return getTotalSeconds() % 60; }

  // Print the current elapsed time as hh:mm:ss
  void printElapsedTime() const {
    int hours = getHoursH();
    int minutes = getMinutesH();
    int seconds = getSecondsH();

    std::cout << (hours < 10 ? "0" : "") << hours << ":"
              << (minutes < 10 ? "0" : "") << minutes << ":"
              << (seconds < 10 ? "0" : "") << seconds << std::endl;
  }
};