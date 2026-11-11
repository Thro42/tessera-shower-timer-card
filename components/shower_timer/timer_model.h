#pragma once
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

namespace esphome::shower_timer {

// Pure logic, also compiled by tests/test_model.cpp. No local heating/timer actions.
inline bool duration_seconds(const char *text, uint32_t &seconds) {
  if (text == nullptr || *text == '\0') return false;
  uint64_t parts[3] = {0, 0, 0};
  int count = 0;
  const char *p = text;
  while (*p && count < 3) {
    if (*p < '0' || *p > '9') return false;
    do {
      parts[count] = parts[count] * 10 + static_cast<unsigned>(*p++ - '0');
      if (parts[count] > 359999) return false;
    } while (*p >= '0' && *p <= '9');
    ++count;
    if (!*p) break;
    if (*p++ != ':' || !*p) return false;
  }
  if (*p || count < 2) return false;
  // HA uses H:MM:SS; also accept MM:SS from a compact fixture.
  if (parts[count - 1] > 59 || (count == 3 && parts[1] > 59)) return false;
  uint64_t value = count == 3 ? parts[0] * 3600 + parts[1] * 60 + parts[2] : parts[0] * 60 + parts[1];
  if (value > 359999) return false;  // bounded to 99:59:59
  seconds = static_cast<uint32_t>(value);
  return true;
}

inline std::string countdown_text(uint32_t seconds) {
  char text[16];
  // Minutes deliberately do not wrap at 60: the user's one-hour timer starts at 60:00.
  snprintf(text, sizeof(text), "%02u:%02u", static_cast<unsigned>(seconds / 60),
           static_cast<unsigned>(seconds % 60));
  return text;
}

enum class TimerMode { MISSING, IDLE, ACTIVE, PAUSED };
struct TimerModel {
  TimerMode mode = TimerMode::MISSING;
  uint32_t finish = 0, paused = 0;
  bool paused_valid = false;

  void update(const char *state, uint32_t finishes_at, const char *remaining) {
    finish = finishes_at;
    paused_valid = duration_seconds(remaining, paused);
    mode = !strcmp(state, "idle") ? TimerMode::IDLE : !strcmp(state, "active") ? TimerMode::ACTIVE :
           !strcmp(state, "paused") ? TimerMode::PAUSED : TimerMode::MISSING;
  }
  bool left(uint32_t now, uint32_t &seconds) const {
    if (mode == TimerMode::IDLE) { seconds = 0; return true; }
    if (mode == TimerMode::PAUSED && paused_valid) { seconds = paused; return true; }
    // An active timer's remaining attribute is NOT live; never count from that snapshot.
    if (mode != TimerMode::ACTIVE || !finish || !now) return false;
    seconds = finish > now ? finish - now : 0;
    return true;
  }
};

inline bool temperature_value(const char *text, float &value) {
  if (!text || !*text) return false;
  char *end = nullptr;
  value = strtof(text, &end);
  if (end == text || !std::isfinite(value)) return false;
  while (*end == ' ' || *end == '\t') ++end;
  return !*end && value >= -273.15f && value <= 10000.0f;
}

}  // namespace esphome::shower_timer
