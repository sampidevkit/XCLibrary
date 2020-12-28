#ifndef DATALOG_H
#define	DATALOG_H

#include "LibDef.h"
#include "datalog_cfg.h"

#ifndef LOG_SIZE
#define LOG_SIZE        512// 512B data log
#endif

#ifndef FRAME_DATA_SIZE
#define FRAME_DATA_SIZE 20 
#endif

typedef struct FRAME_CONTEXT {
    uint32_t Len;
    uint8_t Framed[2 * FRAME_DATA_SIZE + 2];
} __attribute_packed__ frame_t;

typedef union DATALOG_QUEUE_STATUS {

    struct {
        uint8_t Full : 1;
        uint8_t Empty : 1;
        uint8_t RFU : 6;
    } Bits;
    uint8_t Val;
} datalog_status_t;

typedef struct DATALOG_QUEUE_CONTEXT {
    volatile uint32_t Head;
    volatile uint32_t Tail;
    volatile uint8_t Data[LOG_SIZE];
    volatile datalog_status_t State;
} datalog_t;

void DataLog_Init(void);
void DataLog_Reset(void);
uint8_t DataLog_Get(void);
uint32_t DataLog_GetSize(void);
void DataLog_Put(uint8_t Data);
void DataLog_Putfa(float *pData, uint32_t Len);
void DataLog_Put2f(float Data1, float Data2);
void DataLog_Put3f(float Data1, float Data2, float Data3);
void DataLog_Put5f(float Data1, float Data2, float Data3, float Data4, float Data5);
void DataLog_Putia(uint32_t *pData, uint32_t Len);
void DataLog_Put2i(uint32_t Data1, uint32_t Data2);
void DataLog_Put3i(uint32_t Data1, uint32_t Data2, uint32_t Data3);
void DataLog_Put5i(uint32_t Data1, uint32_t Data2, uint32_t Data3, uint32_t Data4, uint32_t Data5);
void DataLog_Task(void);

#endif