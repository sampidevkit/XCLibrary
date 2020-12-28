#include "HC595.h"
#include "libcomp.h"

static uint8_t Hc595Buf[NUM_OF_74HC595];

#ifndef HC595_Spi_Open
#define HC595_Spi_Open()
#endif

#ifndef HC595_Spi_Close
#define HC595_Spi_Close()
#endif

#ifndef HC595_NMR_SetHigh
#define HC595_NMR_SetHigh()
#endif

#ifndef HC595_NMR_SetLow
#define HC595_NMR_SetLow()
#endif

#ifndef HC595_NOE_SetHigh
#define HC595_NOE_SetHigh()
#endif

#ifndef HC595_NOE_SetLow
#define HC595_NOE_SetLow()
#endif

#ifndef HC595_Spi_Xfer

private void HC595_Spi_Xfer(uint8_t Data) // <editor-fold defaultstate="collapsed" desc="HC595 SPI write">
{
    uint8_t i;

    for(i=0; i<8; i++)
    {
        HC595_SHCP_SetLow();

        if(Data&0x80)
            HC595_DS_SetHigh();
        else
            HC595_DS_SetLow();

        Data<<=1;
        HC595_SHCP_SetHigh();
    }
} // </editor-fold>
#endif

public void HC595_Write(uint8_t Idx, uint8_t Value) // <editor-fold defaultstate="collapsed" desc="HC595 write">
{
    uint8_t i;

    if(Idx>=NUM_OF_74HC595)
        return;

    HC595_Spi_Open();
    Hc595Buf[Idx]=Value;
    HC595_NCS_SetLow();

    for(i=0; i<NUM_OF_74HC595; i++)
        HC595_Spi_Xfer(Hc595Buf[i]);

    HC595_NCS_SetHigh();
    HC595_Spi_Close();
} // </editor-fold>

public void HC595_Init(void) // <editor-fold defaultstate="collapsed" desc="HC595 init">
{
    int i;

    HC595_NCS_SetHigh();
    HC595_NMR_SetHigh(); // Running mode
    HC595_NOE_SetLow(); // Enbale
    memset(Hc595Buf, 0x00, NUM_OF_74HC595);

    for(i=0; i<NUM_OF_74HC595; i++)
        HC595_Write(i, 0x00);

} // </editor-fold>