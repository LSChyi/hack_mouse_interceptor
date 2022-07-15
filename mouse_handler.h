#ifndef MOUSE_HANDLER_H_
#define MOUSE_HANDLER_H_

#define DEFAULT_TRIGGER_TIME_VALUE 500

#include <stdint.h>

enum class State {
  Release,
  Pressed,
  Triggering,
  TriggerReleasing,
};

class MouseHandler {
public:
  MouseHandler()
      : left_state_(false), middle_state_(false), right_state_(false),
        state_(State::Release), trigger_time_(0),
        trigger_time_value_(DEFAULT_TRIGGER_TIME_VALUE){};
  void Init();
  void OnLeftReport(bool pressing);
  void OnMiddleReport(bool pressing);
  void OnRightReport(bool pressing);
  void OnPosReport(uint8_t x, uint8_t y, uint8_t wheel);
  void Task();
  void SetTriggerThreshold(uint32_t time);

private:
  bool left_state_;
  bool middle_state_;
  bool right_state_;
  State state_;
  unsigned long trigger_time_;
  uint16_t trigger_time_value_;
};

#endif
