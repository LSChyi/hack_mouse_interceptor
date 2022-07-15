//#define DEBUG // Turn on if need debugging

#include "modified_hidescriptorparser.h"

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
}

void loop() { Usb.Task(); }
