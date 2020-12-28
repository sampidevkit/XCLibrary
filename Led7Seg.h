#ifndef LED7SEG_H
#define LED7SEG_H

#include "LibDef.h"
#include "Led7Seg_Cfg.h"

#if !defined(USE_LED_7SEG_CA) && !defined(USE_LED_7SEG_CC)
#warning "Use default configure: USE_LED_7SEG_CA"
#define USE_LED_7SEG_CA
#endif

#if defined(USE_LED_7SEG_CA)&& defined(USE_LED_7SEG_CC)
#error "Only select USE_LED_7SEG_CA or USE_LED_7SEG_CC"
#endif

extern const uint8_t Led7SegCode[];

public void Led7Seg_Init(void); // Call first
public void Led7Seg_Tasks(void);
public void Led7Seg_SetDigitValue(uint8_t DgIdx, uint8_t Value);
public void Led7Seg_DisplayInteger(uint32_t Value);
public void Led7Seg_DisplayFloat(uint32_t Value, uint8_t DotIdx);
public void Led7Seg_DigitToggleEnable(uint8_t DgIdx);
public void Led7Seg_DigitToggleDisable(uint8_t DgIdx);
public void Led7Seg_DisplayTime(uint8_t HH, uint8_t MM, uint8_t SS);

#endif