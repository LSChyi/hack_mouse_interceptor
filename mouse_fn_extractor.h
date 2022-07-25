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
  bool GetLeftBtn(uint8_t *buf) { return (buf[offset_bits_ / 8] & 0x1); };
  bool GetRightBtn(uint8_t *buf) { return (buf[offset_bits_ / 8] & 0x2); };
  bool GetMiddleBtn(uint8_t *buf) { return (buf[offset_bits_ / 8] & 0x4); };
};

class MousePosExtractor : public MouseFnExtractor {
public:
  int8_t GetX(uint8_t *buf) { return GetValue(buf, offset_bits_); };
  int8_t GetY(uint8_t *buf) { return GetValue(buf, offset_bits_ + size_); };

private:
  // HID report are little endian
  int8_t GetValue(uint8_t *buf, uint8_t offset_bits) {
    // current implementation accept at most 16 bits X or Y report value.
    // due to bit alignment issue, almost all mouse report X or Y  with 8, 16,
    // or 12 bits.
    int16_t val = 0;
    switch (size_) {
    case 8:
      return buf[offset_bits / 8];
    case 16:
      val |= (buf[offset_bits / 8] | (buf[(offset_bits / 8) + 1] << 8));
      break;
    case 12:
      if (offset_bits % 8 == 0) {
        val |= (buf[offset_bits / 8] | ((buf[(offset_bits / 8) + 1] & 0x0F) << 8));
      } else {
        val |= ((buf[offset_bits / 8] >> 4) | (buf[(offset_bits / 8) + 1] << 4));
      }
      val <<= 4; // shift sign bit to match first bit
      val /= (1 << 4); // divide back
      break;
    }

    if (val >= 127) {
      return 127;
    } else if (val <= -127) {
      return -127;
    } else {
      return val;
    }
  };
};

class MouseWheelExtractor : public MouseFnExtractor {
public:
  int8_t GetWheel(uint8_t *buf) { return buf[offset_bits_ / 8]; };
};

#endif
