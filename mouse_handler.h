#ifndef MOUSE_HANDLER_H_
#define MOUSE_HANDLER_H_

#include <stdint.h>

class MouseHandler {
public:
 void Init();
 void OnLeftReport(bool pressing);
 void OnMiddleReport(bool pressing);
 void OnRightReport(bool pressing);
 void OnPosReport(uint8_t x, uint8_t y, uint8_t wheel);
private:
 bool left_state_;
 bool middle_state_;
 bool right_state_;
};

#endif
