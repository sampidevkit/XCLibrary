#ifndef HC595_H
#define HC595_H

#include "LibDef.h"
#include "HC595_Cfg.h"

public void HC595_Init(void);
public void HC595_Write(uint8_t Idx, uint8_t Value);

#endif

