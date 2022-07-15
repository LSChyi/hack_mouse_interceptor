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
public:
  HIDSelector(USB *p) : HIDComposite(p){};

protected:
  bool SelectInterface(uint8_t iface, uint8_t proto);
};

// Return true for the interface we want to hook into
bool HIDSelector::SelectInterface(uint8_t iface, uint8_t proto) {
  return (proto != 0) ? true : false;
}

USB Usb;
HIDSelector hidSelector(&Usb);
ModifiedParser::UniversalReportParser Uni;
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

  if (!hidSelector.SetReportParser(0, &Uni)) {
    ErrorMessage<uint8_t>(PSTR("SetReportParser"), 1);
  }

  mouse_handler.Init();
  Uni.SetMouseHandler(&mouse_handler);

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
