#include "TextLCD.h"
#include "libcomp.h"

private uint8_t MaxLine=2;
private uint8_t MaxRow=8;
private const uint8_t DDRAM_Start_Address[4]={0x80, 0xC0, 0x90, 0xD0};

private void TextLCD_4Bits_Write(char nib) // <editor-fold defaultstate="collapsed" desc="Write 4-bit data">
{
    if(nib&1)
        TEXTLCD_D4_SetHigh();
    else
        TEXTLCD_D4_SetLow();
    nib>>=1;

    if(nib&1)
        TEXTLCD_D5_SetHigh();
    else
        TEXTLCD_D5_SetLow();
    nib>>=1;

    if(nib&1)
        TEXTLCD_D6_SetHigh();
    else
        TEXTLCD_D6_SetLow();
    nib>>=1;

    if(nib&1)
        TEXTLCD_D7_SetHigh();
    else
        TEXTLCD_D7_SetLow();
} // </editor-fold>

private void TextLCD_Enable(void) // <editor-fold defaultstate="collapsed" desc="LCD enable">
{
    TEXTLCD_EN_SetHigh();
    __delay_us(3);
    TEXTLCD_EN_SetLow();
    __delay_us(50);
} // </editor-fold>

private void TextLCD_CMD(char cmd) // <editor-fold defaultstate="collapsed" desc="Send command">
{
    TextLCD_4Bits_Write(cmd>>4);
    TextLCD_Enable();
    TextLCD_4Bits_Write(cmd);
    TextLCD_Enable();
} // </editor-fold>

public void TextLCD_PutChar(char data) // <editor-fold defaultstate="collapsed" desc="Put a char">
{
    TEXTLCD_RS_SetHigh();
    TextLCD_CMD(data);
    TEXTLCD_RS_SetLow();
} // </editor-fold>

public void TextLCD_SetCursor(uint8_t line, uint8_t idx) // <editor-fold defaultstate="collapsed" desc="Set cursor">
{
    uint8_t address;

    if(line>MaxLine)
        line=MaxLine;

    if(idx>MaxRow)
        idx=MaxRow;

    address=DDRAM_Start_Address[line]+idx;
    TextLCD_CMD(address);
    __delay_us(50);
} // </editor-fold>

public void TextLCD_Puts(uint8_t line, uint8_t idx, const char *data, uint8_t length_or_type) // <editor-fold defaultstate="collapsed" desc="Put a string">
{
    TextLCD_SetCursor(line, idx);

    if(length_or_type==0)
        while(*data!=0)
            TextLCD_PutChar(*data++);
    else
    {
        while(length_or_type>0)
        {
            TextLCD_PutChar(*data++);
            length_or_type--;
        }
    }
} // </editor-fold>

public void TextLCD_Clear(void) // <editor-fold defaultstate="collapsed" desc="Clear LCD">
{
    TextLCD_CMD(0x01);
    __delay_ms(2);
} // </editor-fold>

public void TextLCD_ClearLine(uint8_t line) // <editor-fold defaultstate="collapsed" desc="Clear 1 line">
{
    TextLCD_Puts(line, 0, "                ", TEXTLCD_PUT_STRING);
} // </editor-fold>

public bool TextLCD_Init(textlcd_t TextLcdType) // <editor-fold defaultstate="collapsed" desc="LCD initialize">
{
    switch(TextLcdType)
    {
        case TEXTLCD0802:
            MaxLine=2;
            MaxRow=8;
            break;

        case TEXTLCD0804:
            MaxLine=2;
            MaxRow=8;
            break;

        case TEXTLCD1602:
            MaxLine=2;
            MaxRow=16;
            break;

        case TEXTLCD1604:
            MaxLine=4;
            MaxRow=16;
            break;

        case TEXTLCD2002:
            MaxLine=2;
            MaxRow=20;
            break;

        case TEXTLCD2004:
            MaxLine=4;
            MaxRow=20;
            break;

        default:
            return 1; // failed
    }

    __delay_ms(40);
    TextLCD_4Bits_Write(0x03);
    TextLCD_Enable();
    __delay_ms(5);
    TextLCD_Enable();
    __delay_us(100);
    TextLCD_Enable();
    TextLCD_4Bits_Write(0x02);
    TextLCD_Enable();
    TextLCD_CMD(0x28);
    TextLCD_CMD(0x0c);
    TextLCD_CMD(0x06);
    TextLCD_CMD(0x01);
    __delay_ms(100);

    return 0;
} // </editor-fold>