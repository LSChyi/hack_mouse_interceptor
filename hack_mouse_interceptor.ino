//#define DEBUG // Turn on if need debugging
#define TRIGGER_VALUE_ADDR 0

#include "modified_hidescriptorparser.h"
#include "mouse_handler.h"

#include <EEPROM.h>
#include <SPI.h>
#include <hidcomposite.h>
#include <usbhub.h>

// Override HIDComposite to be able to select which interface we want to hook
// into
class HIDSelector : public HIDComposite {
  uint8_t target_interface_;

public:
  HIDSelector(USB *p) : HIDComposite(p){};

protected:
  bool SelectInterface(uint8_t iface, uint8_t proto);
  uint8_t OnInitSuccessful();
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
  ModifiedParser::ReportDescParser report_parser;
  return GetReportDescr(target_interface_, &report_parser);
}

USB Usb;
HIDSelector hidSelector(&Usb);
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
