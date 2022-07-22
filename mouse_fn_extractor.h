#ifndef MOUSE_FN_EXTRACTOR_H_
#define MOUSE_FN_EXTRACTOR_H_

#include <Arduino.h>
#include <stdint.h>

class MouseFnExtractor {
public:
  MouseFnExtractor()
      : size_(0), count_(0), offset_bits_(0), is_able_set_values_(true){};
  // Some reports may require dummy padding, so unless `Reset` is called
  // explicitly, set values will act as one time writable storage.
  void SetValues(uint8_t size, uint8_t count, uint8_t offset_bits) {
    if (!is_able_set_values_)
      return;
    size_ = size;
    count_ = count;
    offset_bits_ = offset_bits;
    is_able_set_values_ = false;
  };
  uint32_t ExtractValue(uint8_t *buf, uint8_t len) { return 0; };
  void Reset() {
    size_ = 0;
    count_ = 0;
    offset_bits_ = 0;
    is_able_set_values_ = true;
  };

protected:
  uint8_t size_;
  uint8_t count_;
  uint8_t offset_bits_;
  bool is_able_set_values_;
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
