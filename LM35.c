#include "LM35.h"
#include "libcomp.h"

private uint8_t i;
public int16_t __LM35_Temper[NUM_OF_LM35]; // extern variable
private const uint8_t PosChannel[NUM_OF_LM35]={
    LM35_1_ADC_POS_CHANNEL,

#if(NUM_OF_LM35>=2)
    LM35_2_ADC_POS_CHANNEL,
#endif

#if(NUM_OF_LM35>=3)
    LM35_3_ADC_POS_CHANNEL,
#endif

#if(NUM_OF_LM35>=4)
    LM35_4_ADC_POS_CHANNEL,
#endif

#if(NUM_OF_LM35>=5)
    LM35_5_ADC_POS_CHANNEL,
#endif

#if(NUM_OF_LM35>=6)
    LM35_6_ADC_POS_CHANNEL,
#endif

#if(NUM_OF_LM35>=7)
    LM35_7_ADC_POS_CHANNEL,
#endif

#if(NUM_OF_LM35>=8)
    LM35_8_ADC_POS_CHANNEL,
#endif
};

private const uint8_t NegChannel[NUM_OF_LM35]={
    LM35_1_ADC_NEG_CHANNEL,

#if(NUM_OF_LM35>=2)
    LM35_2_ADC_NEG_CHANNEL,
#endif

#if(NUM_OF_LM35>=3)
    LM35_3_ADC_NEG_CHANNEL,
#endif

#if(NUM_OF_LM35>=4)
    LM35_4_ADC_NEG_CHANNEL,
#endif

#if(NUM_OF_LM35>=5)
    LM35_5_ADC_NEG_CHANNEL,
#endif

#if(NUM_OF_LM35>=6)
    LM35_6_ADC_NEG_CHANNEL,
#endif

#if(NUM_OF_LM35>=7)
    LM35_7_ADC_NEG_CHANNEL,
#endif

#if(NUM_OF_LM35>=8)
    LM35_8_ADC_NEG_CHANNEL,
#endif
};

void LM35_Tasks(void)
{
    int32_t tmp=LM35_GetADC(PosChannel[i]);
    int32_t tmp0=LM35_GetADC(NegChannel[i]);

    tmp-=tmp0;
    tmp*=LM35_ADC_VREF;
    tmp/=LM35_ADC_RES;

    tmp0=tmp%10;
    
    if(tmp0>=5)
        tmp+=5;
    else if(tmp0<=(-5))
        tmp-=5;

    __LM35_Temper[i]=(int16_t) (tmp/10);

    if(++i>=NUM_OF_LM35)
        i=0;
}

void LM35_Init(void)
{
    for(i=0; i<NUM_OF_LM35; i++)
        __LM35_Temper[i]=0;

    i=0;
}