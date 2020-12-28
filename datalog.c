#include "datalog.h"
#include "libcomp.h"

static datalog_t DataLog;
static frame_t Frame;

void DataLog_Init(void) // <editor-fold defaultstate="collapsed" desc="DataLog initialize">
{
    DataLog_Reset();
} // </editor-fold>

void DataLog_Reset(void) // <editor-fold defaultstate="collapsed" desc="DataLog rest buffer">
{
    DataLog.Head=0;
    DataLog.Tail=0;
    DataLog.State.Bits.Empty=true;
    DataLog.State.Bits.Full=false;
} // </editor-fold>

uint8_t DataLog_Get(void) // <editor-fold defaultstate="collapsed" desc="DataLog read 1 byte from buffer">
{
    uint8_t data=0;

    data=DataLog.Data[DataLog.Head++];

    if(DataLog.Head>=LOG_SIZE)
        DataLog.Head=0;

    if(DataLog.Head==DataLog.Tail)
        DataLog.State.Bits.Empty=true;

    DataLog.State.Bits.Full=false;

    return data;
} // </editor-fold>

uint32_t DataLog_GetSize(void) // <editor-fold defaultstate="collapsed" desc="DataLog get buffer size">
{
    if(!DataLog.State.Bits.Full)
    {
        if(DataLog.Tail>=DataLog.Head)
            return (DataLog.Tail-DataLog.Head);
        else
            return (LOG_SIZE-DataLog.Head+DataLog.Tail);
    }

    return LOG_SIZE;
} // </editor-fold>

void DataLog_Put(uint8_t Data) // <editor-fold defaultstate="collapsed" desc="DataLog write byte to buffer">
{
    DataLog.Data[DataLog.Tail++]=Data;

    if(DataLog.Tail>=LOG_SIZE)
        DataLog.Tail=0;

    DataLog.State.Bits.Empty=false;

    if(DataLog.Tail==DataLog.Head)
        DataLog.State.Bits.Full=true;
} // </editor-fold>

void DataLog_Putfa(float *pData, uint32_t Len) // <editor-fold defaultstate="collapsed" desc="DataLog write float array to buffer">
{
    uint8_t c;
    uint32_t i;

    Len<<=2; // 1 float=4bytes
    DataLog_Put(0xFE);

    for(i=0; i<Len; i++)
    {
        c=((uint8_t *) pData)[i];

        switch(c)
        {
            case 0xFC:
            case 0xFD:
            case 0xFE:
                DataLog_Put(0xFD);
                DataLog_Put(c^0x20);
                break;

            default:
                DataLog_Put(c);
                break;
        }
    }

    DataLog_Put(0xFC);
} // </editor-fold>

void DataLog_Put2f(float Data1, float Data2) // <editor-fold defaultstate="collapsed" desc="DataLog write float array to buffer">
{
    uint8_t c;
    uint32_t i, j;
    float tmp;

    DataLog_Put(0xFE);

    for(i=0; i<2; i++)
    {
        switch(i)
        {
            case 0:
                tmp=Data1;
                break;

            case 1:
                tmp=Data2;
                break;

            default:
                return;
        }

        for(j=0; j<4; j++)
        {
            c=((uint8_t *) (&tmp))[j];

            switch(c)
            {
                case 0xFC:
                case 0xFD:
                case 0xFE:
                    DataLog_Put(0xFD);
                    DataLog_Put(c^0x20);
                    break;

                default:
                    DataLog_Put(c);
                    break;
            }
        }
    }

    DataLog_Put(0xFC);
} // </editor-fold>

void DataLog_Put3f(float Data1, float Data2, float Data3) // <editor-fold defaultstate="collapsed" desc="DataLog write float array to buffer">
{
    uint8_t c;
    uint32_t i, j;
    float tmp;

    DataLog_Put(0xFE);

    for(i=0; i<3; i++)
    {
        switch(i)
        {
            case 0:
                tmp=Data1;
                break;

            case 1:
                tmp=Data2;
                break;

            case 2:
                tmp=Data3;
                break;

            default:
                return;
        }

        for(j=0; j<4; j++)
        {
            c=((uint8_t *) (&tmp))[j];

            switch(c)
            {
                case 0xFC:
                case 0xFD:
                case 0xFE:
                    DataLog_Put(0xFD);
                    DataLog_Put(c^0x20);
                    break;

                default:
                    DataLog_Put(c);
                    break;
            }
        }
    }

    DataLog_Put(0xFC);
} // </editor-fold>

void DataLog_Put5f(float Data1, float Data2, float Data3, float Data4, float Data5) // <editor-fold defaultstate="collapsed" desc="DataLog write float array to buffer">
{
    uint8_t c;
    uint32_t i, j;
    float tmp;

    DataLog_Put(0xFE);

    for(i=0; i<5; i++)
    {
        switch(i)
        {
            case 0:
                tmp=Data1;
                break;

            case 1:
                tmp=Data2;
                break;

            case 2:
                tmp=Data3;
                break;

            case 3:
                tmp=Data4;
                break;

            case 4:
                tmp=Data5;
                break;

            default:
                return;
        }

        for(j=0; j<4; j++)
        {
            c=((uint8_t *) (&tmp))[j];

            switch(c)
            {
                case 0xFC:
                case 0xFD:
                case 0xFE:
                    DataLog_Put(0xFD);
                    DataLog_Put(c^0x20);
                    break;

                default:
                    DataLog_Put(c);
                    break;
            }
        }
    }

    DataLog_Put(0xFC);
} // </editor-fold>

void DataLog_Putia(uint32_t *pData, uint32_t Len) // <editor-fold defaultstate="collapsed" desc="DataLog write integer array to buffer">
{
    uint8_t c;
    uint32_t i;

    Len<<=2; // 1 float=4bytes
    DataLog_Put(0xFE);

    for(i=0; i<Len; i++)
    {
        c=((uint8_t *) pData)[i];

        switch(c)
        {
            case 0xFC:
            case 0xFD:
            case 0xFE:
                DataLog_Put(0xFD);
                DataLog_Put(c^0x20);
                break;

            default:
                DataLog_Put(c);
                break;
        }
    }

    DataLog_Put(0xFC);
} // </editor-fold>

void DataLog_Put2i(uint32_t Data1, uint32_t Data2) // <editor-fold defaultstate="collapsed" desc="DataLog write integer array to buffer">
{
    uint8_t c;
    uint32_t i, j;
    uint32_t tmp;

    DataLog_Put(0xFE);

    for(i=0; i<2; i++)
    {
        switch(i)
        {
            case 0:
                tmp=Data1;
                break;

            case 1:
                tmp=Data2;
                break;

            default:
                return;
        }

        for(j=0; j<4; j++)
        {
            c=((uint8_t *) (&tmp))[j];

            switch(c)
            {
                case 0xFC:
                case 0xFD:
                case 0xFE:
                    DataLog_Put(0xFD);
                    DataLog_Put(c^0x20);
                    break;

                default:
                    DataLog_Put(c);
                    break;
            }
        }
    }

    DataLog_Put(0xFC);
} // </editor-fold>

void DataLog_Put3i(uint32_t Data1, uint32_t Data2, uint32_t Data3) // <editor-fold defaultstate="collapsed" desc="DataLog write integer array to buffer">
{
    uint8_t c;
    uint32_t i, j;
    uint32_t tmp;

    DataLog_Put(0xFE);

    for(i=0; i<3; i++)
    {
        switch(i)
        {
            case 0:
                tmp=Data1;
                break;

            case 1:
                tmp=Data2;
                break;

            case 2:
                tmp=Data3;
                break;

            default:
                return;
        }

        for(j=0; j<4; j++)
        {
            c=((uint8_t *) (&tmp))[j];

            switch(c)
            {
                case 0xFC:
                case 0xFD:
                case 0xFE:
                    DataLog_Put(0xFD);
                    DataLog_Put(c^0x20);
                    break;

                default:
                    DataLog_Put(c);
                    break;
            }
        }
    }

    DataLog_Put(0xFC);
} // </editor-fold>

void DataLog_Put5i(uint32_t Data1, uint32_t Data2, uint32_t Data3, uint32_t Data4, uint32_t Data5) // <editor-fold defaultstate="collapsed" desc="DataLog write integer array to buffer">
{
    uint8_t c;
    uint32_t i, j;
    uint32_t tmp;

    DataLog_Put(0xFE);

    for(i=0; i<5; i++)
    {
        switch(i)
        {
            case 0:
                tmp=Data1;
                break;

            case 1:
                tmp=Data2;
                break;

            case 2:
                tmp=Data3;
                break;

            case 3:
                tmp=Data4;
                break;

            case 4:
                tmp=Data5;
                break;

            default:
                return;
        }

        for(j=0; j<4; j++)
        {
            c=((uint8_t *) (&tmp))[j];

            switch(c)
            {
                case 0xFC:
                case 0xFD:
                case 0xFE:
                    DataLog_Put(0xFD);
                    DataLog_Put(c^0x20);
                    break;

                default:
                    DataLog_Put(c);
                    break;
            }
        }
    }

    DataLog_Put(0xFC);
} // </editor-fold>

void DataLog_Task(void) // <editor-fold defaultstate="collapsed" desc="DataLog task">
{
    static uint32_t DoNext=0;
    static uint8_t TxData=0;

    switch(DoNext)
    {
        case 0:
            if(DataLog.State.Bits.Empty==0)// Buffer is not empty
            {
                TxData=DataLog_Get();
                DoNext=1;
            }
            else
                break;

        case 1:
            if(DATALOG_Port_is_tx_ready())
            {
                DATALOG_Port_Write(TxData);
                DoNext=0;
            }
        default:
            break;

    }
} // </editor-fold>