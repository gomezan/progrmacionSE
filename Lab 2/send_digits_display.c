
#include "Varios.h"

char print_digit(char digit);

char send_digits_display(char digit, char flag_25, char flag_CA1, char flag_off, char flag_TO10)
{
    static cont = 0;

    if(digit == 0x23 || digit == 0x24 || digit == 0x25 || digit == 0x26)
        return FALSE;

    if(flag_CA1 == 1)
        digit = ~digit;

    digit &= 0x0F;

    if(flag_25 == 1)
    {
        cont++;
        if(cont > 3)
        {
            cont = 0;
        }
        else
        {
            // Apagado
            digit = 0xF0;
        }
    }
    else
    {
        cont = 0;
    }

    if(flag_off)
    {
        // Apagado
        digit = 0xF0;
    }

    if(flag_TO10 == 1)
    {
        // guión
        digit = 0xFF;
    }
    
    return print_digit(digit);

}