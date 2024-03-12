
#include "Varios.h"

char print_digit(char digit);

char send_digits_display(char digit, char flag_25, char flag_CA1)
{
    static cont = 0;
    if(flag_25 == 1)
    {
        cont++;
        if(cont > 3)
        {
            cont = 0;
        }
    }
    else
    {
        cont = 0;
    }

    if(digit == 0x23 || digit == 0x24 || digit == 0x25 || digit == 0x26)
        return FALSE;

    if(flag_CA1 == 1)
        digit = ~digit;
    
    return print_digit(digit&0x0F);

}