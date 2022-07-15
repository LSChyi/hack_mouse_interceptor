#ifndef MOUSE_FN_EXTRACTOR_H_
#define MOUSE_FN_EXTRACTOR_H_

#include <Arduino.h>
#include <stdint.h>

class MouseFnExtractor {
protected:
  uint8_t size_;
  uint8_t count_;
  uint32_t value_;

public:
  void SetValues(uint8_t size, uint8_t count, uint32_t value) {
    size_ = size;
    count_ = count;
    value_ = value;
  };
  uint32_t GetValue() { return value_; };
};

class MouseBtnExtractor : public MouseFnExtractor {
public:
  bool GetBtn() { return value_; };
};

class MousePosExtractor : public MouseFnExtractor {
public:
  uint8_t GetPos() { return ((value_ >> (size_ - 8)) & 0xFF); };
};

class MouseWheelExtractor : public MouseFnExtractor {
public:
  uint8_t GetWheel() { return value_ & 0xFF; };
};

#endif
