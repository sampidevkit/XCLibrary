#ifndef LEDMATRIX_H
#define LEDMATRIX_H

#include "LibDef.h"
#include "LedMatrix_Cfg.h"

/* ******************************************** Configure Template in RpLibCfg.h
#define NUMBER_OF_ROWS          8
#define NUMBER_OF_COLS          8
#define NUMBER_OF_PIXELS        64
#define SCANNING_ROW_OR_COL     1 // 0: Row scanning, 1 Col scanning
#define LED_MATRIX_ACT_TIME     2 // ms, Fscan=1E3/(NUMBER_OF_COLS*LED_MATRIX_ACT_TIME)=1E3/(8*3)=62.5Hz
#define LED_MATRIX_TOGGLE_COUNT 125// Toggle time=LED_MATRIX_TOGGLE_COUNT*LED_MATRIX_ACT_TIME=250ms

#if(SCANNING_ROW_OR_COL==0)
#define LedMatrix_Row_SetState(x)   (PORTD=~(1<<x))
#define LedMatrix_Col_SetState(x)   (PORTB=x)
#else
#define LedMatrix_Row_SetState(x)   (PORTD=~x)
#define LedMatrix_Col_SetState(x)   (PORTB=(1<<x))
#endif
 **************************************************************************** */

#if(NUMBER_OF_ROWS<=8)
#define row_size_t uint8_t
#elif(NUMBER_OF_ROWS<=16)
#define row_size_t uint16_t
#else
#error "This library doesn't support more than 16 rows"
#endif

#if(NUMBER_OF_COLS<=8)
#define col_size_t uint8_t
#elif(NUMBER_OF_COLS<=16)
#define col_size_t uint16_t
#else
#error "This library doesn't support more than 16 cols"
#endif

#define LED_MATRIX_NO_TOGGLE    0x0000
#define LED_MATRIX_TOGGLE       0x8000
#define LED_MATRIX_TOGGLE_LATCH 0x4000
#define LED_MATRIX_TOGGLE_MASK  0xC000
#define LED_MATRIX_INDEX_MASK   0x1FFF
#define LED_MATRIX_BACKUP_BIT   0x2000
#define LED_MATRIX_BACKUP_MASK  0xDFFF
#define LED_MATRIX_IDX_BK_MASK  0x3FFF

#if(SCANNING_ROW_OR_COL) // Col scanning mode
public void LedMatrix_Init(void); // Call first
public void LedMatrix_Tasks(void); // Not need to call, it is automatically added to Task_Manager() by LedMatrix_Init()
public void LedMatrix_SetRow(row_size_t RowIdx, row_size_t Value);
public void LedMatrix_SetCol(col_size_t ColIdx, col_size_t Value);
public void LedMatrix_SetPixel(uint16_t PixelIdx, bool Value);
public bool LedMatrix_GetPixel(uint16_t PixelIdx);
public void LedMatrix_SetAll(bool Value);
public void LedMatrix_TogglePixelEnable(uint16_t PixelIdx);
public void LedMatrix_TogglePixelDisable(uint16_t PixelIdx);
#else // Row scanning mode
public void LedMatrix_Init(void); // Call first
public void LedMatrix_Tasks(void); // Not need to call, it is automatically added to Task_Manager() by LedMatrix_Init()
public void LedMatrix_SetCol(col_size_t ColIdx, col_size_t Value);
public void LedMatrix_SetRow(row_size_t RowIdx, row_size_t Value);
public void LedMatrix_SetPixel(uint16_t PixelIdx, bool Value);
public bool LedMatrix_GetPixel(uint16_t PixelIdx);
public void LedMatrix_SetAll(bool Value);
public void LedMatrix_TogglePixelEnable(uint16_t PixelIdx);
public void LedMatrix_TogglePixelDisable(uint16_t PixelIdx);
#endif

#endif

