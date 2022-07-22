#define DEBUG // Turn on if need debugging
#define TRIGGER_VALUE_ADDR 0

#include "modified_hidescriptorparser.h"
#include "mouse_fn_extractor.h"
#include "mouse_handler.h"

#include <EEPROM.h>
#include <SPI.h>
#include <hidcomposite.h>
#include <usbhub.h>

// Override HIDComposite to be able to select which interface we want to hook
// into
class HIDSelector : public HIDComposite {
public:
  HIDSelector(USB *p, MouseBtnExtractor *btn_extractor,
              MousePosExtractor *pos_extractor,
              MouseWheelExtractor *wheel_extractor)
      : HIDComposite(p), btn_extractor_(btn_extractor),
        pos_extractor_(pos_extractor), wheel_extractor_(wheel_extractor){};

protected:
  bool SelectInterface(uint8_t iface, uint8_t proto);
  void ParseHIDData(USBHID *hid, uint8_t ep, bool is_rpt_id, uint8_t len,
                    uint8_t *buf); // Called by the HIDComposite library
  uint8_t OnInitSuccessful();

private:
  uint8_t target_interface_;
  MouseBtnExtractor *btn_extractor_;
  MousePosExtractor *pos_extractor_;
  MouseWheelExtractor *wheel_extractor_;
};

// Return true for the interface we want to hook into
bool HIDSelector::SelectInterface(uint8_t iface, uint8_t proto) {
  if (proto == 2) {
    target_interface_ = iface;
    return true;
  }
  return false;
}

uint8_t HIDSelector::OnInitSuccessful() {
  ModifiedParser::ReportDescParser report_parser(btn_extractor_, pos_extractor_,
                                                 wheel_extractor_);
  return GetReportDescr(target_interface_, &report_parser);
}

void HIDSelector::ParseHIDData(USBHID *hid, uint8_t ep, bool is_rpt_id,
                               uint8_t len, uint8_t *buf) {
  if (len && buf) {
    Serial.print(F("\r\n"));
    for (uint8_t i = 0; i < len; i++) {
      if (buf[i] < 16)
        Serial.print(F("0"));
      Serial.print(buf[i], HEX);
      Serial.print(F(" "));
    }
  }
}

MouseBtnExtractor btn_extractor;
MousePosExtractor pos_extractor;
MouseWheelExtractor wheel_extractor;

USB Usb;
HIDSelector hidSelector(&Usb, &btn_extractor, &pos_extractor, &wheel_extractor);
MouseHandler mouse_handler;
uint32_t trigger_value;

void setup() {
  Serial.begin(115200);
#ifdef DEBUG
  while (!Serial)
    ;
#endif
  Serial.println("Start");

  if (Usb.Init() == -1) {
    Serial.println("OSC did not start.");
  }

  delay(200);

  mouse_handler.Init();

  EEPROM.get(TRIGGER_VALUE_ADDR, trigger_value);
  Serial.print("trigger ms set to ");
  Serial.println(trigger_value);
  mouse_handler.SetTriggerThreshold(trigger_value);
}

void loop() {
  Usb.Task();
  mouse_handler.Task();

  if (Serial.available()) {
    int trigger_value_input = Serial.parseInt();
    if (trigger_value_input != 0) {
      trigger_value = trigger_value_input;
      EEPROM.put(TRIGGER_VALUE_ADDR, trigger_value);
      mouse_handler.SetTriggerThreshold(trigger_value);
      Serial.print("trigger ms set to ");
      Serial.println(trigger_value);
    } else {
      Serial.println("enter trigger ms: ");
    }
  }
}
