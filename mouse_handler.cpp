#include "mouse_handler.h"

#include <Mouse.h>
#include <stdint.h>

void MouseHandler::Init() { Mouse.begin(); }

void MouseHandler::OnLeftReport(bool pressing) {
  if (left_state_ != pressing) {
    pressing ? Mouse.press(MOUSE_LEFT) : Mouse.release(MOUSE_LEFT);
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
