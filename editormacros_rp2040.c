/* SPDX-License-Identifier: GPL-2.0-or-later */

#include "editormacros_rp2040.h"

#ifdef RGB_MATRIX_ENABLE
led_config_t g_led_config = {{
    //Key Matrix to LED Index
    {NO_LED,NO_LED,NO_LED,NO_LED},
    {3,2,1,0},
    {4,5,6,7},
    {11,10,9,8},
    {NO_LED,NO_LED,NO_LED,NO_LED}
},{
    // LED Index to Physical Position
    {222,0},{148, 0},{74,0},{0,0},{0,32},{74,32},{148,32},{222,32},{0,64},{74,64},{148,64},{222,64}
},{
    // LED Index to Flag
    4,4,4,4,4,4,4,4,4,4,4,4
}};
#endif
