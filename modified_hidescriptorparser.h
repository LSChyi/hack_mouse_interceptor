/* Copyright (C) 2011 Circuits At Home, LTD. All rights reserved.

This software may be distributed and modified under the terms of the GNU
General Public License version 2 (GPL2) as published by the Free Software
Foundation and appearing in the file GPL2.TXT included in the packaging of
this file. Please note that GPL2 Section 2[b] requires that all works based
on this software must also be made publicly available under the terms of
the GPL2 ("Copyleft").

Contact information
-------------------

Circuits At Home, LTD
Web      :  http://www.circuitsathome.com
e-mail   :  support@circuitsathome.com
 */
#ifndef MODIFIED_HIDESCRIPTORPARSER_H_
#define MODIFIED_HIDESCRIPTORPARSER_H_

#include "mouse_fn_extractor.h"

#include <usbhid.h>

namespace ModifiedParser {
class ReportDescParserBase : public USBReadParser {
public:
  typedef void (*UsagePageFunc)(uint16_t usage);

  static void PrintGenericDesktopPageUsage(uint16_t usage);
  static void PrintSimulationControlsPageUsage(uint16_t usage);
  static void PrintVRControlsPageUsage(uint16_t usage);
  static void PrintSportsControlsPageUsage(uint16_t usage);
  static void PrintGameControlsPageUsage(uint16_t usage);
  static void PrintGenericDeviceControlsPageUsage(uint16_t usage);
  static void PrintLEDPageUsage(uint16_t usage);
  static void PrintButtonPageUsage(uint16_t usage);
  static void PrintOrdinalPageUsage(uint16_t usage);
  static void PrintTelephonyPageUsage(uint16_t usage);
  static void PrintConsumerPageUsage(uint16_t usage);
  static void PrintDigitizerPageUsage(uint16_t usage);
  static void PrintAlphanumDisplayPageUsage(uint16_t usage);
  static void PrintMedicalInstrumentPageUsage(uint16_t usage);

  static void PrintValue(uint8_t *p, uint8_t len);
  static void PrintByteValue(uint8_t data);

  static void PrintItemTitle(uint8_t prefix);

  static const char *const usagePageTitles0[];
  static const char *const usagePageTitles1[];
  static const char *const genDesktopTitles0[];
  static const char *const genDesktopTitles1[];
  static const char *const genDesktopTitles2[];
  static const char *const genDesktopTitles3[];
  static const char *const genDesktopTitles4[];
  static const char *const simuTitles0[];
  static const char *const simuTitles1[];
  static const char *const simuTitles2[];
  static const char *const vrTitles0[];
  static const char *const vrTitles1[];
  static const char *const sportsCtrlTitles0[];
  static const char *const sportsCtrlTitles1[];
  static const char *const sportsCtrlTitles2[];
  static const char *const gameTitles0[];
  static const char *const gameTitles1[];
  static const char *const genDevCtrlTitles[];
  static const char *const ledTitles[];
  static const char *const telTitles0[];
  static const char *const telTitles1[];
  static const char *const telTitles2[];
  static const char *const telTitles3[];
  static const char *const telTitles4[];
  static const char *const telTitles5[];
  static const char *const consTitles0[];
  static const char *const consTitles1[];
  static const char *const consTitles2[];
  static const char *const consTitles3[];
  static const char *const consTitles4[];
  static const char *const consTitles5[];
  static const char *const consTitles6[];
  static const char *const consTitles7[];
  static const char *const consTitles8[];
  static const char *const consTitles9[];
  static const char *const consTitlesA[];
  static const char *const consTitlesB[];
  static const char *const consTitlesC[];
  static const char *const consTitlesD[];
  static const char *const consTitlesE[];
  static const char *const digitTitles0[];
  static const char *const digitTitles1[];
  static const char *const digitTitles2[];
  static const char *const aplphanumTitles0[];
  static const char *const aplphanumTitles1[];
  static const char *const aplphanumTitles2[];
  static const char *const medInstrTitles0[];
  static const char *const medInstrTitles1[];
  static const char *const medInstrTitles2[];
  static const char *const medInstrTitles3[];
  static const char *const medInstrTitles4[];

protected:
  static UsagePageFunc usagePageFunctions[];

  MultiValueBuffer theBuffer;
  MultiByteValueParser valParser;
  ByteSkipper theSkipper;
  uint8_t varBuffer[sizeof(USB_CONFIGURATION_DESCRIPTOR)];

  uint8_t itemParseState; // Item parser state variable
  uint8_t itemSize;       // Item size
  uint8_t itemPrefix;     // Item prefix (first byte)
  uint8_t rptSize;        // Report Size
  uint8_t rptCount;       // Report Count

  uint16_t totalSize; // Report size in bits

  // Method should be defined here if virtual.
  virtual uint8_t ParseItem(uint8_t **pp, uint16_t *pcntdn);

  UsagePageFunc pfUsage;

  static void PrintUsagePage(uint16_t page);
  void SetUsagePage(uint16_t page);

public:
  ReportDescParserBase(MouseFnExtractor *btn_extractor,
                       MouseFnExtractor *pos_extractor,
                       MouseFnExtractor *wheel_extractor)
      : itemParseState(0), itemSize(0), itemPrefix(0), rptSize(0), rptCount(0),
        pfUsage(NULL), accumulated_offset_bits_(0),
        btn_extractor_(btn_extractor), pos_extractor_(pos_extractor),
        wheel_extractor_(wheel_extractor) {
    theBuffer.pValue = varBuffer;
    valParser.Initialize(&theBuffer);
    theSkipper.Initialize(&theBuffer);
  };

  void Parse(const uint16_t len, const uint8_t *pbuf, const uint16_t &offset);

  enum {
    enErrorSuccess = 0,
    enErrorIncomplete // value or record is partialy read in buffer
    ,
    enErrorBufferTooSmall
  };

private:
  uint8_t accumulated_offset_bits_;
  MouseFnExtractor *btn_extractor_;
  MouseFnExtractor *pos_extractor_;
  MouseFnExtractor *wheel_extractor_;
};

class ReportDescParser : public ReportDescParserBase {
public:
  ReportDescParser(MouseFnExtractor *btn_extractor,
                   MouseFnExtractor *pos_extractor,
                   MouseFnExtractor *wheel_extractor)
      : ReportDescParserBase(btn_extractor, pos_extractor, wheel_extractor){};
};
} // namespace ModifiedParser

#endif // __HIDDESCRIPTORPARSER_H__
