#include "mouse_handler.h"

#include <Arduino.h>
#include <Mouse.h>
#include <stdint.h>

void MouseHandler::Init() { Mouse.begin(); }

void MouseHandler::OnLeftReport(bool pressing) {
  if (left_state_ != pressing) {
    switch (state_) {
    case State::Release: // can only be pressed
      Mouse.press(MOUSE_LEFT);
      state_ = State::Pressed;
      trigger_time_ = millis();
      break;
    case State::Pressed: // can only be released
      Mouse.release(MOUSE_LEFT);
      state_ = State::Release;
      break;
    case State::Triggering:
      if (pressing) {
        state_ = State::TriggerReleasing;
      }
      break;
    case State::TriggerReleasing:
      Mouse.release(MOUSE_LEFT);
      state_ = State::Release;
      break;
    }
  }
  left_state_ = pressing;
}

void MouseHandler::OnMiddleReport(bool pressing) {
  if (middle_state_ != pressing) {
    pressing ? Mouse.press(MOUSE_MIDDLE) : Mouse.release(MOUSE_MIDDLE);
  }
  middle_state_ = pressing;
}

void MouseHandler::OnRightReport(bool pressing) {
  if (right_state_ != pressing) {
    pressing ? Mouse.press(MOUSE_RIGHT) : Mouse.release(MOUSE_RIGHT);
  }
  right_state_ = pressing;
}

void MouseHandler::OnPosReport(uint8_t x, uint8_t y, uint8_t wheel) {
  Mouse.move(x, y, wheel);
}

void MouseHandler::Task() {
  if (state_ == State::Pressed) {
    if (millis() - trigger_time_ >= trigger_time_value_) {
      state_ = State::Triggering;
    }
  }
}
