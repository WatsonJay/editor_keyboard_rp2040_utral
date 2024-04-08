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

#    ifdef MATRIX_ROW_PINS
static SPLIT_MUTABLE_ROW pin_t row_pins[ROWS_PER_HAND] = MATRIX_ROW_PINS;
#    endif // MATRIX_ROW_PINS
#    ifdef MATRIX_COL_PINS
static SPLIT_MUTABLE_COL pin_t col_pins[MATRIX_COLS]   = MATRIX_COL_PINS;
#    endif // MATRIX_COL_PINS

void matrix_init_custom(void) {
    // TODO: initialize hardware here
}

static bool read_rows_on_col(matrix_row_t current_matrix[], uint8_t current_col) {
    select_col(current_col);
    matrix_io_delay();
}

bool matrix_scan_custom(matrix_row_t current_matrix[]) {
    bool matrix_has_changed = false;

    // TODO: add matrix scanning routine here
    for (uint8_t current_col = 0; current_col < MATRIX_COLS; current_col++) {
        matrix_has_changed = read_rows_on_col(curr_matrix, current_col, row_shifter);
    }

    return matrix_has_changed;
}
