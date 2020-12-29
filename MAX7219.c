#include "MAX7219.h"
#include "libcomp.h"

static const char dg[10]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x27, 0x7F, 0x6F}; // Ma led 7 doan (am cuc chung), used in none decode mode
static uint8_t display_data[NUM_OF_MAX7219];

void MAX7219_Write(uint8_t chip_idx, uint8_t addr, uint8_t data)
{
    uint8_t i;

    MAX7219_SelectChip();

    display_data[chip_idx]=data;

    for(i=0; i<NUM_OF_MAX7219; i++)
    {
        MAX7219_SPI_Write(addr);
        MAX7219_SPI_Write(display_data[i]);
    }

    MAX7219_DeselectChip();
}

void MAX7219_Init(const max7219_config_t *pCfgCxt)
{
    MAX7219_Write(pCfgCxt->ChipIdx, REG_DECODE_MODE, pCfgCxt->Decode);
    MAX7219_Write(pCfgCxt->ChipIdx, REG_INTENSITY, pCfgCxt->Intensity);
    MAX7219_Write(pCfgCxt->ChipIdx, REG_SCAN_LIMIT, pCfgCxt->ScanLimit);
    MAX7219_Write(pCfgCxt->ChipIdx, REG_SHUTDOWN, pCfgCxt->Shutdown);
    display_data[pCfgCxt->ChipIdx]=0x00; // off all segments
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
