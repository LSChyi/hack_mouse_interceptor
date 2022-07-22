#ifndef MOUSE_FN_EXTRACTOR_H_
#define MOUSE_FN_EXTRACTOR_H_

#include <Arduino.h>
#include <stdint.h>

class MouseFnExtractor {
public:
  MouseFnExtractor() : size_(0), count_(0), offset_bits_(0){};
  void SetValues(uint8_t size, uint8_t count, uint8_t offset_bits) {
    size_ = size;
    count_ = count;
    offset_bits_ = offset_bits;
  };
  uint32_t ExtractValue(uint8_t *buf, uint8_t len) { return 0; };

protected:
  uint8_t size_;
  uint8_t count_;
  uint8_t offset_bits_;
};

class MouseBtnExtractor : public MouseFnExtractor {
public:
};

class MousePosExtractor : public MouseFnExtractor {
public:
};

class MouseWheelExtractor : public MouseFnExtractor {
public:
};

#endif
