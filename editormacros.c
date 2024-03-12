#include "editormacros.h"

#ifdef RGB_MATRIX_ENABLE
led_config_t g_led_config = {{
    {NO_LED,NO_LED,1,0,NO_LED},
    {3,2,6,7,8},
    {4,5,11,10,9},
    {NO_LED,NO_LED,12,13,14},
    {NO_LED,NO_LED,NO_LED,NO_LED}
},{
    {168,0},{112,0},{56,0},{0,0},{0,21},{56,21},{112,21},{168,21},{224,21},{224,43},{168,43},{112,43},{112,64},{168,64},{224,64}
},{
    4, 4, 4, 4, 4, 4, 4, 4,4, 4, 4, 4, 4, 4, 4
}};
#endif
