#include "Led7Seg.h"
#include "libcomp.h"

#ifdef USE_LED_7SEG_CA
#ifdef USER_LED_7SEG_CA
public const uint8_t Led7SegCode[]={USE_LED_7SEG_CA};
#else
/* Bit 0: segment A
 * Bit 1: segment B
 * Bit 1: segment C
 * Bit 1: segment D
 * Bit 1: segment E
 * Bit 1: segment F
 * Bit 1: segment G
 * Bit 1: segment H */
public const uint8_t Led7SegCode[]={
    0xC0, // 0
    0xF9, // 1
    0xA4, // 2
    0xB0, // 3
    0x99, // 4
    0x92, // 5
    0x82, // 6
    0xD8, // 7
    0x80, // 8
    0x90, // 9
    0x88, // A
    0x83, // b
    0xC6, // c
    0xA1, // d
    0x86, // E
    0x8E, // F
    0x7F, // .
    0xFF, // Off all segments
    0x00 // On all segments
};
#endif
#endif

#ifdef USE_LED_7SEG_CC
#ifdef USER_LED_7SEG_CC
public const uint8_t Led7SegCode[]={USE_LED_7SEG_CC};
#else
/* Bit 0: segment A
 * Bit 1: segment B
 * Bit 1: segment C
 * Bit 1: segment D
 * Bit 1: segment E
 * Bit 1: segment F
 * Bit 1: segment G
 * Bit 1: segment H */
public const uint8_t Led7SegCode[]={
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x27, // 7
    0x7F, // 8
    0x6F, // 9
    0x77, // A
    0x7C, // b
    0x39, // c
    0x5E, // d
    0x79, // E
    0x71, // F
    0x80, // .
    0x00, // Off all segments
    0xFF // On all segments
};
#endif
#endif

private uint8_t DigitIdx=0;
private uint8_t Led7SegBuf[NUM_OF_7SEG_DIGIT];
private uint8_t Led7SegBufBk1[NUM_OF_7SEG_DIGIT];
private uint8_t Led7SegBufBk2[NUM_OF_7SEG_DIGIT];
private uint8_t ToggleCount=0;
private tick_timer_t TickLed7Seg={1, 0, 0};

public void Led7Seg_Tasks(void) // <editor-fold defaultstate="collapsed" desc="7-segment Led tasks">
{
    if(Tick_Timer_Is_Over_Ms(TickLed7Seg, LED_7SEG_ACT_TIME))
    {
        Led7Seg_Digit_Disable(); // Off all digit

#if(USE_LED_7SEG_CC) 
        Led7Seg_Segment_SetState(Led7SegBuf[DigitIdx]&Led7SegBufBk1[DigitIdx]);
#else
        Led7Seg_Segment_SetState(Led7SegBuf[DigitIdx]|Led7SegBufBk1[DigitIdx]);
#endif
        Led7Seg_Digit_SetState(DigitIdx++);

        if(DigitIdx>=NUM_OF_7SEG_DIGIT)
            DigitIdx=0;

        if(++ToggleCount>=LED_7SEG_TOGGLE_COUNT)
        {
            uint8_t i;

            for(i=0; i<NUM_OF_7SEG_DIGIT; i++)
            {
                if(Led7SegBufBk1[i]!=Led7SegBufBk2[i])
                {
                    uint8_t tmp;

                    tmp=Led7SegBufBk1[i];
                    Led7SegBufBk1[i]=Led7SegBufBk2[i];
                    Led7SegBufBk2[i]=tmp;
                }
            }

            ToggleCount=0;
        }
    }
} // </editor-fold>

public void Led7Seg_SetDigitValue(uint8_t DgIdx, uint8_t Value) // <editor-fold defaultstate="collapsed" desc="Set digit value">
{
    if(DgIdx>=NUM_OF_7SEG_DIGIT)
        return;

    Led7SegBuf[DgIdx]=Value;
} // </editor-fold>

public void Led7Seg_DisplayInteger(uint32_t Value) // <editor-fold defaultstate="collapsed" desc="Display integer">
{
    uint8_t i, j;

    j=(NUM_OF_7SEG_DIGIT-1);

    for(i=0; i<NUM_OF_7SEG_DIGIT; i++)
    {
        Led7SegBuf[j]=Led7SegCode[(uint8_t) (Value%10)];
        Value/=10;
        j--;
    }

    for(i=0; i<NUM_OF_7SEG_DIGIT; i++)
    {
        if(Led7SegBuf[i]==Led7SegCode[0])
        {
            if(i<(NUM_OF_7SEG_DIGIT-1))
                Led7SegBuf[i]=Led7SegCode[17];
        }
        else
            break;
    }
} // </editor-fold>

public void Led7Seg_DisplayFloat(uint32_t Value, uint8_t DotIdx) // <editor-fold defaultstate="collapsed" desc="Display float">
{
    if((DotIdx==0)||(DotIdx>=NUM_OF_7SEG_DIGIT))
        return;

    Led7Seg_DisplayInteger(Value);
#if(USE_LED_7SEG_CC)
    Led7SegBuf[DotIdx]|=Led7SegCode[16];
#else
    Led7SegBuf[DotIdx]&=Led7SegCode[16];
#endif
} // </editor-fold>

public void Led7Seg_DisplayTime(uint8_t HH, uint8_t MM, uint8_t SS) // <editor-fold defaultstate="collapsed" desc="Diplay time">
{
    Led7SegBuf[0]=Led7SegCode[HH/10];

    if(Led7SegBuf[0]==Led7SegCode[0])
        Led7SegBuf[0]=Led7SegCode[17]; // Off

    Led7SegBuf[1]=Led7SegCode[HH%10];
    Led7SegBuf[2]=Led7SegCode[MM/10];
    Led7SegBuf[3]=Led7SegCode[MM%10];
#if(NUM_OF_7SEG_DIGIT>=6)
    Led7SegBuf[4]=Led7SegCode[SS/10];
    Led7SegBuf[5]=Led7SegCode[SS%10];
#endif
} // </editor-fold>

public void Led7Seg_DigitToggleEnable(uint8_t DgIdx) // <editor-fold defaultstate="collapsed" desc="Digit toggle enable">
{
    Led7SegBufBk1[DgIdx]=Led7SegCode[17];
    Led7SegBufBk2[DgIdx]=Led7SegCode[18];
} // </editor-fold>

public void Led7Seg_DigitToggleDisable(uint8_t DgIdx) // <editor-fold defaultstate="collapsed" desc="Digit toggle disable">
{
    Led7SegBufBk1[DgIdx]=Led7SegCode[18];
    Led7SegBufBk2[DgIdx]=Led7SegCode[18];
} // </editor-fold>

public void Led7Seg_Init(void) // <editor-fold defaultstate="collapsed" desc="7-segment Led initialize">
{
    Disable_Global_Interrupt();
    memset((void *) Led7SegBuf, Led7SegCode[17], NUM_OF_7SEG_DIGIT);
    memset((void *) Led7SegBufBk1, Led7SegCode[18], NUM_OF_7SEG_DIGIT);
    memset((void *) Led7SegBufBk2, Led7SegCode[18], NUM_OF_7SEG_DIGIT);
    DigitIdx=0;
    ToggleCount=0;
    Enable_Global_Interrupt();
} // </editor-fold>