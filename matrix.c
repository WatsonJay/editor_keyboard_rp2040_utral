/*
Copyright 2012-2018 JayWatson

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "matrix.h"
#include "quantum.h"
#include "analog.h"
#include "print.h"
#include "paj7620.h"

#define _VRY GP27
#define _VRX GP28
#define _JOYSTICK_ROW 4
#define _JOYSTICK_COL_UP 0
#define _JOYSTICK_COL_DOWN 1
#define _JOYSTICK_COL_LEFT 2
#define _JOYSTICK_COL_RIGHT 3
#define _PAJ7620_ROW1 5
#define _PAJ7620_ROW2 6

#ifndef MATRIX_INPUT_PRESSED_STATE
#    define MATRIX_INPUT_PRESSED_STATE 0
#endif

// 键盘默认为 row2col
static const pin_t row_pins[MATRIX_ROWS] = MATRIX_ROW_PINS;
static const pin_t col_pins[MATRIX_COLS] = MATRIX_COL_PINS;
static int actuation = 256; // actuation point for arrows (0-511)

static inline void setPinOutput_writeLow(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinOutput(pin);
        writePinLow(pin);
    }
}

static inline void setPinOutput_writeHigh(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinOutput(pin);
        writePinHigh(pin);
    }
}

static inline void setPinInputHigh_atomic(pin_t pin) {
    ATOMIC_BLOCK_FORCEON {
        setPinInputHigh(pin);
    }
}

static inline uint8_t readMatrixPin(pin_t pin) {
    if (pin != NO_PIN) {
        return (readPin(pin) == MATRIX_INPUT_PRESSED_STATE) ? 0 : 1;
    } else {
        return 1;
    }
}

static bool select_col(uint8_t col)
{
    if (col_pins[col] != NO_PIN) {
        setPinOutput_writeLow(col_pins[col]);
        return true;
    }
    return false;
}

static bool unselect_col(uint8_t col)
{
    if (col_pins[col] != NO_PIN) {
        setPinInputHigh_atomic(col_pins[col]);
        return true;
    }
    return false;
}

static void unselect_cols(void)
{
    for(uint8_t x = 0; x < MATRIX_COLS; x++) {
        if (col_pins[x] != NO_PIN) {
            setPinInputHigh_atomic(col_pins[x]);
        }
    }
}

static void read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col, matrix_row_t row_shifter)
{
    bool key_pressed = false;
    // Select col and wait for col selecton to stabilize
    if (!select_col(current_col)) { // Select row
        return;                     // skip NO_PIN row
    }
    matrix_io_delay();

    // For each row...
    for(uint8_t row_index = 0; row_index < MATRIX_ROWS; row_index++)
    {
        if (row_index == _PAJ7620_ROW1|| row_index == _PAJ7620_ROW2) {
            continue;
        }
        switch (row_index) {
            case _JOYSTICK_ROW:
                if (
                    (current_col == _JOYSTICK_COL_UP    && analogReadPin(_VRY) - 512 < -actuation) ||  //up
                    (current_col == _JOYSTICK_COL_DOWN  && analogReadPin(_VRY) - 512 > actuation) ||  //down
                    (current_col == _JOYSTICK_COL_LEFT  && analogReadPin(_VRX) - 512 < -actuation) ||  //LEFT
                    (current_col == _JOYSTICK_COL_RIGHT && analogReadPin(_VRX) - 512 > actuation)  //RIGHT
                ) {
                    // Pin LO, set col bit
                    current_matrix[row_index] |= row_shifter;
                    key_pressed = true;
                } else {
                    current_matrix[row_index] &= ~row_shifter;
                }
                break;
            default:
                // Check row pin state
                if (readMatrixPin(row_pins[row_index]) == 0)
                {
                    // Pin LO, set col bit
                    current_matrix[row_index] |= row_shifter;
                    key_pressed = true;
                }
                else
                {
                    // Pin HI, clear col bit
                    current_matrix[row_index] &= ~row_shifter;
                }
        }
    }

    // Unselect col
    unselect_col(current_col);
    matrix_output_unselect_delay(current_col, key_pressed);
}

static void set_paj7620_to_matrix(matrix_row_t current_matrix[]) {
    current_matrix[_PAJ7620_ROW1] = 0b00000;
    current_matrix[_PAJ7620_ROW2] = 0b00000;
    uint16_t gesture = paj7620_gesture();
    //注意向右旋转90度
    // up -> right
    // down -> left
    // right -> down
    // left -> up
    switch(gesture) {
        case GES_RIGHT_FLAG: //up
            current_matrix[_PAJ7620_ROW1] = 0b00001;break;
        case GES_LEFT_FLAG: //down
            current_matrix[_PAJ7620_ROW1] = 0b00010;break;
        case GES_DOWN_FLAG: //right
            current_matrix[_PAJ7620_ROW1] = 0b00100;break;
        case GES_UP_FLAG: //left
            current_matrix[_PAJ7620_ROW1] = 0b01000;break;
        case GES_WAVE_FLAG: //wave
            current_matrix[_PAJ7620_ROW1] = 0b10000;break;
        case GES_FORWARD_FLAG: //forward
            current_matrix[_PAJ7620_ROW2] = 0b00001;break;
        case GES_BACKWARD_FLAG: //backward
            current_matrix[_PAJ7620_ROW2] = 0b00010;break;
        case GES_CLOCKWISE_FLAG: //clockwise
            current_matrix[_PAJ7620_ROW2] = 0b00100;break;
        case GES_COUNT_CLOCKWISE_FLAG: //countCLockwise
            current_matrix[_PAJ7620_ROW2] = 0b01000;break;
        default:
            break;
    }
}

void matrix_init_custom(void) {
    // TODO: initialize hardware here
    unselect_cols();

    for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
        if (row_pins[x] != NO_PIN) {
            setPinInputHigh_atomic(row_pins[x]);
        }
    }

    paj7620_init();
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    static matrix_row_t temp_matrix[MATRIX_ROWS] = {0};
    matrix_row_t row_shifter = MATRIX_ROW_SHIFTER;
    // Set col, read rows
    for (uint8_t current_col = 0; current_col < MATRIX_COLS; current_col++, row_shifter <<= 1) {
        read_rows_on_col(temp_matrix, current_col, row_shifter);
    }
    set_paj7620_to_matrix(temp_matrix);
    bool changed = memcmp(current_matrix, temp_matrix, sizeof(temp_matrix)) != 0;
    if (changed) {
        memcpy(current_matrix, temp_matrix, sizeof(temp_matrix));
    }
    return changed;
}
