#include "../components/shower_timer/timer_model.h"
#include <cassert>
#include <iostream>

using namespace esphome::shower_timer;
int main() {
  unsigned checks = 0;
  auto check = [&](bool value) { assert(value); ++checks; };
  uint32_t seconds = 999;
  check(duration_seconds("1:00:00", seconds) && seconds == 3600);
  check(duration_seconds("00:23:45", seconds) && seconds == 1425);
  check(duration_seconds("60:00", seconds) && seconds == 3600);
  check(duration_seconds("0:00:00", seconds) && seconds == 0);
  for (const char *bad : {"", "unknown", "-1:00:00", "0:60:00", "1:00:60", "1:2:3:4", "0:00:01junk", "1:", "1::2", "999999999999999999999:00:00", "100:00:00"})
    check(!duration_seconds(bad, seconds));
  check(countdown_text(3600) == "60:00");
  check(countdown_text(3599) == "59:59");
  check(countdown_text(0) == "00:00");
  check(countdown_text(7261) == "121:01");
  TimerModel model;
  check(!model.left(1000, seconds));
  model.update("active", 4600, "1:00:00");
  check(model.left(1000, seconds) && seconds == 3600);
  check(model.left(1001, seconds) && seconds == 3599);
  check(model.left(4599, seconds) && seconds == 1);
  check(model.left(4600, seconds) && seconds == 0);
  check(model.left(9999, seconds) && seconds == 0);
  check(!model.left(0, seconds));
  model.update("paused", 0, "0:23:45");
  check(model.left(1000, seconds) && seconds == 1425);
  check(model.left(2000, seconds) && seconds == 1425);
  check(model.left(0, seconds) && seconds == 1425);
  model.update("active", 6000, "0:23:45");
  check(model.left(5000, seconds) && seconds == 1000);  // resume uses a new finish, not old remaining
  model.update("idle", 6000, "0:23:45");
  check(model.left(5000, seconds) && seconds == 0);
  model.update("unavailable", 6000, "0:23:45");
  check(!model.left(5000, seconds));
  model.update("paused", 6000, "broken");
  check(!model.left(5000, seconds));
  model.update("active", 0, "1:00:00");
  check(!model.left(5000, seconds));  // a non-live remaining snapshot must never be used for active
  float temperature;
  check(temperature_value("40.5", temperature) && temperature == 40.5f);
  check(temperature_value("-10.0", temperature) && temperature == -10.0f);
  check(temperature_value("0", temperature) && temperature == 0.0f);
  for (const char *bad : {"", "unknown", "unavailable", "nan", "inf", "-inf", "40junk", "40.5°C", "-300", "10001"})
    check(!temperature_value(bad, temperature));
  std::cout << checks << " checks passed: timer parsing, countdown, pause/resume, expiry, missing clock/data and temperature\n";
}
