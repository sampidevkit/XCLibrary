#ifndef LM35_H
#define	LM35_H

#include "LibDef.h"
#include "LM35_Cfg.h"

#if(NUM_OF_LM35>8)
#error "This driver supports up to 8 sensors"
#endif

extern int16_t __LM35_Temper[NUM_OF_LM35];

void LM35_Init(void);
void LM35_Tasks(void);
#define LM35_GetTemper(Idx) __LM35_Temper[Idx]

#endif