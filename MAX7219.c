#include "MAX7219.h"
#include "libcomp.h"

#define __debug(msg, val) //do{DbWriteStr(msg), DbByte2Hex(val);}while(0)

static const char dg[10]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F}; // Ma led 7 doan (am cuc chung), used in none decode mode
static uint8_t display_data[NUM_OF_MAX7219][8];

#ifndef MAX7219_Xfer

uint8_t MAX7219_Xfer(uint8_t c)
{
    uint8_t i, k;

    for(i=0, k=0; i<8; i++)
    {
        MAX7219_CLK_SetLow();

        k<<=1;

        if(MAX7219_DOUT_GetValue())
            k|=1;

        if(c&0x80)
            MAX7219_DIN_SetHigh();
        else
            MAX7219_DIN_SetLow();

        MAX7219_CLK_SetHigh();
        c<<=1;
    }

    c=k;
    
    return c;
}
#else
#define MAX7219_CLK_SetHigh()       
#define MAX7219_CLK_SetLow()        
#define MAX7219_DIN_SetHigh()       
#define MAX7219_DIN_SetLow()      
#define MAX7219_DOUT_GetValue() 0
#endif

void MAX7219_Write(uint8_t chip_idx, uint8_t addr, uint8_t data)
{
    uint8_t i;

    MAX7219_LOAD_Enable();
    // Modify
    display_data[chip_idx][addr-1]=data;
    // Write
    for(i=0; i<NUM_OF_MAX7219; i++)
    {
        data=MAX7219_Xfer(addr);
        data=MAX7219_Xfer(display_data[i][addr-1]);
    }

    MAX7219_LOAD_Disable();
}

void MAX7219_Config(const max7219_config_t *pCfgCxt)
{
    MAX7219_Write(pCfgCxt->ChipIdx, REG_DECODE_MODE, pCfgCxt->Decode);
    MAX7219_Write(pCfgCxt->ChipIdx, REG_INTENSITY, pCfgCxt->Intensity);
    MAX7219_Write(pCfgCxt->ChipIdx, REG_SCAN_LIMIT, pCfgCxt->ScanLimit);
    MAX7219_Write(pCfgCxt->ChipIdx, REG_SHUTDOWN, pCfgCxt->Shutdown);
}

void MAX7219_Display(const max7219_config_t *pCfgCxt, uint32_t Num)
{
    uint8_t i;

    for(i=pCfgCxt->ScanLimit+2; i>0; i--)
    {
        uint8_t temp=(uint8_t) (Num%10);

        Num/=10;
        MAX7219_Write(pCfgCxt->ChipIdx, i-1, dg[temp]);
    }
}

void MAX7219_Init(void)
{
    MAX7219_LOAD_Disable();
    MAX7219_CLK_SetHigh();
    MAX7219_DIN_SetLow();
}