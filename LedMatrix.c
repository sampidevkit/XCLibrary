#include "libcomp.h"
#include "LedMatrix.h"

#if(SCANNING_ROW_OR_COL==1) // Col scanning mode
/* ***************************************************************** MATRIX MAPS
             [COL0][COL1][COL2][COL3][COL4][COL5][COL6][COL7][....][COLM]
 [Bit0][ROW0][  x ][  x ][  x ][  x ][  x ][  x ][  x ][  x ][....][  x ]
 [Bit1][ROW1][  x ][  x ][  x ][  x ][  x ][  x ][  x ][  x ][....][  x ]
 [Bit2][ROW2][  x ][  x ][  x ][  x ][  x ][  x ][  x ][  x ][....][  x ]
 [Bit3][ROW3][  x ][  x ][  x ][  x ][  x ][  x ][  x ][  x ][....][  x ]
 [Bit4][ROW4][  x ][  x ][  x ][  x ][  x ][  x ][  x ][  x ][....][  x ]
 [Bit5][ROW5][  x ][  x ][  x ][  x ][  x ][  x ][  x ][  x ][....][  x ]
 [Bit6][ROW6][  x ][  x ][  x ][  x ][  x ][  x ][  x ][  x ][....][  x ]
 [Bit7][ROW7][  x ][  x ][  x ][  x ][  x ][  x ][  x ][  x ][....][  x ]
 [....][....][....][....][....][....][....][....][....][....][....][....]
 [BitN][ROWN][  x ][  x ][  x ][  x ][  x ][  x ][  x ][  x ][....][  x ]
 * ************************************************************************** */

private col_size_t ColIdx=0;
private uint16_t LatchCount=0;
private uint16_t ToggleIdx=LED_MATRIX_NO_TOGGLE;
private tick_timer_t TickLedMatrix={1, 0, 0};
private row_size_t RowDataBuf[NUMBER_OF_COLS];

public void LedMatrix_Init(void) // <editor-fold defaultstate="collapsed" desc="Matrix Led initialize">
{
    Disable_Global_Interrupt();
    memset(RowDataBuf, 0x00, NUMBER_OF_COLS);
    ToggleIdx=LED_MATRIX_NO_TOGGLE;
    LatchCount=0;
    ColIdx=0;
    Tick_Timer_Reset(TickLedMatrix);
    Enable_Global_Interrupt();
} // </editor-fold>

public void LedMatrix_Tasks(void) // <editor-fold defaultstate="collapsed" desc="Matrix Led tasks">
{
    static bool thislatch=0;

    if(thislatch)
    {
        uint16_t tmp;

        thislatch=0;
        LedMatrix_Row_SetState(RowDataBuf[ColIdx]);
        LedMatrix_Col_SetState(ColIdx++);
        //ColIdx++;

        if(++LatchCount>=LED_MATRIX_TOGGLE_COUNT)// ~250ms
        {
            tmp=ToggleIdx&LED_MATRIX_TOGGLE_MASK;

            if(tmp==LED_MATRIX_TOGGLE)
            {
                tmp=ToggleIdx&LED_MATRIX_INDEX_MASK;
                LedMatrix_SetPixel(tmp, 1);
                // Change toggle phase
                ToggleIdx&=LED_MATRIX_IDX_BK_MASK;
                ToggleIdx|=LED_MATRIX_TOGGLE_LATCH;
            }
            else if(tmp==LED_MATRIX_TOGGLE_LATCH)
            {
                tmp=ToggleIdx&LED_MATRIX_INDEX_MASK;
                LedMatrix_SetPixel(tmp, 0);
                // Change toggle phase
                ToggleIdx&=LED_MATRIX_IDX_BK_MASK;
                ToggleIdx|=LED_MATRIX_TOGGLE;
            }

            LatchCount=0;
        }

        if(ColIdx>=NUMBER_OF_COLS)
            ColIdx=0;
    }
    else
    {
        if(Tick_Timer_Is_Over_Ms(TickLedMatrix, LED_MATRIX_ACT_TIME))
        {
            LedMatrix_Row_ClearAll(); // Off all row
            LedMatrix_Col_ClearAll(); // Off all col
            thislatch=1;
        }
    }
} // </editor-fold>

public void LedMatrix_SetRow(row_size_t RowIdx, col_size_t Value) // <editor-fold defaultstate="collapsed" desc="Matrix Led set row value">
{
    col_size_t i, colmask;

    if(RowIdx>=NUMBER_OF_ROWS)
        return;

    colmask=(1<<RowIdx);

    for(i=0; i<NUMBER_OF_COLS; i++)
    {
        if(Value&0x01)
            RowDataBuf[i]|=colmask;
        else
            RowDataBuf[i]&=(~colmask);

        Value>>=1;
    }
} // </editor-fold>

public void LedMatrix_SetCol(col_size_t ColIdx, row_size_t Value) // <editor-fold defaultstate="collapsed" desc="Matrix Led set col value">
{
    if(ColIdx>=NUMBER_OF_COLS)
        return;

    RowDataBuf[ColIdx]=Value;
} // </editor-fold>

public void LedMatrix_SetPixel(uint16_t PixelIdx, bool Value) // <editor-fold defaultstate="collapsed" desc="Matrix Led set pixel state">
{
    col_size_t x, colmask;
    row_size_t y;

    if(PixelIdx<=NUMBER_OF_PIXELS)
    {
        x=PixelIdx/NUMBER_OF_COLS;
        y=PixelIdx%NUMBER_OF_ROWS;
        colmask=(1<<y);

        if(Value)
            RowDataBuf[x]|=colmask;
        else
            RowDataBuf[x]&=(~colmask);
    }
} // </editor-fold>

public bool LedMatrix_GetPixel(uint16_t PixelIdx) // <editor-fold defaultstate="collapsed" desc="Matrix Led get pixel state">
{
    col_size_t x, colmask;
    row_size_t y;

    if(PixelIdx<=NUMBER_OF_PIXELS)
    {
        x=PixelIdx/NUMBER_OF_COLS;
        y=PixelIdx%NUMBER_OF_ROWS;
        colmask=(1<<y);

        if((RowDataBuf[x]&colmask)==colmask)
            return 1;
    }

    return 0;
} // </editor-fold>

public void LedMatrix_SetAll(bool Value) // <editor-fold defaultstate="collapsed" desc="All led are on or off">
{
    ToggleIdx=LED_MATRIX_NO_TOGGLE;

    if(Value)
        memset(RowDataBuf, 0xFF, NUMBER_OF_COLS);
    else
        memset(RowDataBuf, 0x00, NUMBER_OF_COLS);
} // </editor-fold>

public void LedMatrix_TogglePixelEnable(uint16_t PixelIdx) // <editor-fold defaultstate="collapsed" desc="Toggle 1 pixel">
{
    if(PixelIdx<=NUMBER_OF_PIXELS)
    {
        ToggleIdx=LED_MATRIX_TOGGLE|PixelIdx;

        if(LedMatrix_GetPixel(PixelIdx))
            ToggleIdx|=LED_MATRIX_BACKUP_BIT;
    }
} // </editor-fold>

public void LedMatrix_TogglePixelDisable(uint16_t PixelIdx) // <editor-fold defaultstate="collapsed" desc="Stop toggle 1 pixel">
{
    if((ToggleIdx&LED_MATRIX_BACKUP_BIT)==LED_MATRIX_BACKUP_BIT)
        LedMatrix_SetPixel(PixelIdx, 1);
    else
        LedMatrix_SetPixel(PixelIdx, 0);

    ToggleIdx=LED_MATRIX_NO_TOGGLE;
} // </editor-fold>
#else // Row scanning mode
/* ***************************************************************** MATRIX MAPS
       [Bit0][Bit0][Bit0][Bit0][Bit0][Bit0][Bit0][Bit0][....][BitM]
       [COL0][COL1][COL2][COL3][COL4][COL5][COL6][COL7][....][COLM]
 [ROW0][  x ][  x ][  x ][  x ][  x ][  x ][  x ][  x ][....][  x ]
 [ROW1][  x ][  x ][  x ][  x ][  x ][  x ][  x ][  x ][....][  x ]
 [ROW2][  x ][  x ][  x ][  x ][  x ][  x ][  x ][  x ][....][  x ]
 [ROW3][  x ][  x ][  x ][  x ][  x ][  x ][  x ][  x ][....][  x ]
 [ROW4][  x ][  x ][  x ][  x ][  x ][  x ][  x ][  x ][....][  x ]
 [ROW5][  x ][  x ][  x ][  x ][  x ][  x ][  x ][  x ][....][  x ]
 [ROW6][  x ][  x ][  x ][  x ][  x ][  x ][  x ][  x ][....][  x ]
 [ROW7][  x ][  x ][  x ][  x ][  x ][  x ][  x ][  x ][....][  x ]
 [....][....][....][....][....][....][....][....][....][....][....]
 [ROWN][  x ][  x ][  x ][  x ][  x ][  x ][  x ][  x ][....][  x ]
 * ************************************************************************** */

private row_size_t RowIdx=0;
private uint16_t LatchCount=0;
private uint16_t ToggleIdx=LED_MATRIX_NO_TOGGLE;
private tick_timer_t TickLedMatrix={1, 0, 0};
private col_size_t ColDataBuf[NUMBER_OF_ROWS];

public void LedMatrix_Init(void) // <editor-fold defaultstate="collapsed" desc="Matrix Led initialize">
{
    Disable_Global_Interrupt();
    memset(ColDataBuf, 0x00, NUMBER_OF_ROWS);
    ToggleIdx=LED_MATRIX_NO_TOGGLE;
    LatchCount=0;
    RowIdx=0;
    Tick_Timer_Reset(TickLedMatrix);
    Enable_Global_Interrupt();
} // </editor-fold>

public void LedMatrix_Tasks(void) // <editor-fold defaultstate="collapsed" desc="Matrix Led tasks">
{
    static bool thislatch=0;

    if(thislatch)
    {
        uint16_t tmp;

        thislatch=0;
        LedMatrix_Col_SetState(ColDataBuf[RowIdx]);
        LedMatrix_Row_SetState(RowIdx++);

        if(++LatchCount>=LED_MATRIX_TOGGLE_COUNT)// ~250ms
        {
            tmp=ToggleIdx&LED_MATRIX_TOGGLE_MASK;

            if(tmp==LED_MATRIX_TOGGLE)
            {
                tmp=ToggleIdx&LED_MATRIX_INDEX_MASK;
                LedMatrix_SetPixel(tmp, 1);
                // Change toggle phase
                ToggleIdx&=LED_MATRIX_IDX_BK_MASK;
                ToggleIdx|=LED_MATRIX_TOGGLE_LATCH;
            }
            else if(tmp==LED_MATRIX_TOGGLE_LATCH)
            {
                tmp=ToggleIdx&LED_MATRIX_INDEX_MASK;
                LedMatrix_SetPixel(tmp, 0);
                // Change toggle phase
                ToggleIdx&=LED_MATRIX_IDX_BK_MASK;
                ToggleIdx|=LED_MATRIX_TOGGLE;
            }

            LatchCount=0;
        }

        if(RowIdx>=NUMBER_OF_ROWS)
            RowIdx=0;
    }
    else
    {
        if(Tick_Timer_Is_Over_Ms(TickLedMatrix, LED_MATRIX_ACT_TIME))
        {
            LedMatrix_Col_ClearAll(); // Off all col
            LedMatrix_Row_ClearAll(); // Off all row
            thislatch=1;
        }
    }
} // </editor-fold>

public void LedMatrix_SetCol(col_size_t ColIdx, row_size_t Value) // <editor-fold defaultstate="collapsed" desc="Matrix Led set col value">
{
    row_size_t i, rowmask;

    if(ColIdx>=NUMBER_OF_COLS)
        return;

    rowmask=(1<<ColIdx);

    for(i=0; i<NUMBER_OF_ROWS; i++)
    {
        if(Value&0x01)
            ColDataBuf[i]|=rowmask;
        else
            ColDataBuf[i]&=(~rowmask);

        Value>>=1;
    }
} // </editor-fold>

public void LedMatrix_SetRow(row_size_t RowIdx, col_size_t Value) // <editor-fold defaultstate="collapsed" desc="Matrix Led set row value">
{
    if(RowIdx>=NUMBER_OF_ROWS)
        return;

    ColDataBuf[RowIdx]=Value;
} // </editor-fold>

public void LedMatrix_SetPixel(uint16_t PixelIdx, bool Value) // <editor-fold defaultstate="collapsed" desc="Matrix Led set pixel state">
{
    row_size_t x, rowmask;
    col_size_t y;

    if(PixelIdx<=NUMBER_OF_PIXELS)
    {
        x=PixelIdx/NUMBER_OF_ROWS;
        y=PixelIdx%NUMBER_OF_COLS;
        rowmask=(1<<y);

        if(Value)
            ColDataBuf[x]|=rowmask;
        else
            ColDataBuf[x]&=(~rowmask);
    }
} // </editor-fold>

public bool LedMatrix_GetPixel(uint16_t PixelIdx) // <editor-fold defaultstate="collapsed" desc="Matrix Led get pixel state">
{
    row_size_t x, rowmask;
    col_size_t y;

    if(PixelIdx<=NUMBER_OF_PIXELS)
    {
        x=PixelIdx/NUMBER_OF_ROWS;
        y=PixelIdx%NUMBER_OF_COLS;
        rowmask=(1<<y);

        if((ColDataBuf[x]&rowmask)==rowmask)
            return 1;
    }

    return 0;
} // </editor-fold>

public void LedMatrix_SetAll(bool Value) // <editor-fold defaultstate="collapsed" desc="All led are on or off">
{
    ToggleIdx=LED_MATRIX_NO_TOGGLE;

    if(Value)
        memset(ColDataBuf, 0xFF, NUMBER_OF_ROWS);
    else
        memset(ColDataBuf, 0x00, NUMBER_OF_ROWS);
} // </editor-fold>

public void LedMatrix_TogglePixelEnable(uint16_t PixelIdx) // <editor-fold defaultstate="collapsed" desc="Toggle 1 pixel">
{
    if(PixelIdx<=NUMBER_OF_PIXELS)
    {
        ToggleIdx=LED_MATRIX_TOGGLE|PixelIdx;

        if(LedMatrix_GetPixel(PixelIdx))
            ToggleIdx|=LED_MATRIX_BACKUP_BIT;
    }
} // </editor-fold>

public void LedMatrix_TogglePixelDisable(uint16_t PixelIdx) // <editor-fold defaultstate="collapsed" desc="Stop toggle 1 pixel">
{
    if((ToggleIdx&LED_MATRIX_BACKUP_BIT)==LED_MATRIX_BACKUP_BIT)
        LedMatrix_SetPixel(PixelIdx, 1);
    else
        LedMatrix_SetPixel(PixelIdx, 0);

    ToggleIdx=LED_MATRIX_NO_TOGGLE;
} // </editor-fold>

#endif