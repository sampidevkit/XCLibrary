#ifndef MAX7219_H
#define	MAX7219_H

#include "LibDef.h"
#include "MAX7219_Cfg.h"

enum REGISTER_ADDRESS_MAP {
    REG_NO_OP = 0x00,
    REG_DIGIT0,
    REG_DIGIT1,
    REG_DIGIT2,
    REG_DIGIT3,
    REG_DIGIT4,
    REG_DIGIT5,
    REG_DIGIT6,
    REG_DIGIT7,
    REG_DECODE_MODE,
    REG_INTENSITY,
    REG_SCAN_LIMIT,
    REG_SHUTDOWN,
    REG_DISPLAY_TEST = 0x0F
};

typedef enum DECODE_MODE_REGISTER_EXAMPLES {
    NO_DECODE_DIGIT7_0 = 0x00,
    B_DECODE_DIGIT0_NO_DECODE7_1 = 0x01,
    B_DECODE_DIGIT3_0_NO_DECODE7_4 = 0x0F,
    B_DECODE_DIGIT7_0 = 0xFF
} max7219_decode_mode_t;

typedef enum INTENSITY_REGISTER_FORMAT {
    MIN_ON = 0x00,
    DUTY_1_32 = 0x00,
    DUTY_3_32,
    DUTY_5_32,
    DUTY_7_32,
    DUTY_9_32,
    DUTY_11_32,
    DUTY_13_32,
    DUTY_15_32,
    DUTY_17_32,
    DUTY_19_32,
    DUTY_21_32,
    DUTY_23_32,
    DUTY_25_32,
    DUTY_27_32,
    DUTY_29_32,
    DUTY_31_32,
    MAX_ON = 0x0F
} max7219_intensity_t;

typedef enum SCAN_LIMIT_REGISTER_FORMAT {
    DIGIT0_ONLY = 0x00,
    DIGIT0_1,
    DIGIT0_2,
    DIGIT0_3,
    DIGIT0_4,
    DIGIT0_5,
    DIGIT0_6,
    DIGIT0_7
} max7219_scan_limit_t;

typedef enum SHUTDOWN_REGISTER_FORMAT {
    SHUTDOWN_MODE = 0,
    NORMAL_MODE
} max7219_shutdown_t;

typedef struct MAX7219_CONFIG_CONTEXT{
    uint8_t ChipIdx;
    max7219_decode_mode_t Decode;
    max7219_intensity_t Intensity;
    max7219_scan_limit_t ScanLimit;
    max7219_shutdown_t Shutdown;
    struct MAX7219_CONFIG_CONTEXT *pNext;
} max7219_config_t;

void MAX7219_Init(void);
void MAX7219_Config(const max7219_config_t *pCfgCxt);
void MAX7219_Display(const max7219_config_t *pCfgCxt, uint32_t Num);
void MAX7219_Write(uint8_t chip_idx, uint8_t addr, uint8_t data);

#endif