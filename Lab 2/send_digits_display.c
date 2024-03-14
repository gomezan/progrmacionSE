
#include "Varios.h"

char print_digit(char digit)
{
    static char lookup_table[18] = {
        /* xgfe dcba */
        0x3F, // x011 1111 - 0
        0x06, // x000 0110 - 1
        0x5B, // x101 1011 - 2
        0x4F, // x100 1111 - 3
        0x67, // x110 0111 - 4
        0x6D, // x110 1101 - 5
        0x7D, // x111 1101 - 6
        0x07, // x000 0111 - 7
        0x7F, // x111 1111 - 8
        0x6F, // x110 1111 - 9
        0x77, // x111 0111 - A
        0x7C, // x111 1100 - B (b)
        0x39, // x011 1001 - C
        0x5E, // x101 1110 - D (d)
        0x79, // x111 1001 - E
        0x71, // x111 0001 - F
        0x00, // x000 0000 - off
        0x40// x100 0000 - guión
    };
    static unsigned char pins[7] = 
    {
        GPIO_OUTPUT_IO_0,
        GPIO_OUTPUT_IO_1,
        GPIO_OUTPUT_IO_2,
        GPIO_OUTPUT_IO_3,
        GPIO_OUTPUT_IO_4,
        GPIO_OUTPUT_IO_5,
        GPIO_OUTPUT_IO_6
    };

    for(int i = 6; i; i--)
    {
        if(lookup_table[digit] & (1 << i))
        {
            gpio_set_level(pins[i],0);
        }
        else
        {
            gpio_set_level(pins[i],1);
        }

    }
/*
    if(lookup_table[digit] & 0x01) // bit 0 - a
    {
        gpio_set_level(GPIO_OUTPUT_IO_0, 0);
    }
    else
    {
        gpio_set_level(GPIO_OUTPUT_IO_0, 1);
    }


    if(lookup_table[digit] & 0x02) // bit 1 - b
    {
        gpio_set_level(GPIO_OUTPUT_IO_1, 0);
    }
    else
    {
        gpio_set_level(GPIO_OUTPUT_IO_1, 1);
    }

    if(lookup_table[digit] & 0x04) // bit 2 - c
    {
        gpio_set_level(GPIO_OUTPUT_IO_2, 0);
    }
    else
    {
        gpio_set_level(GPIO_OUTPUT_IO_2, 1);
    }

    if(lookup_table[digit] & 0x08) // bit 3 - d
    {
        gpio_set_level(GPIO_OUTPUT_IO_3, 0);
    }
    else
    {
        gpio_set_level(GPIO_OUTPUT_IO_3, 1);
    }

    if(lookup_table[digit] & 0x10) // bit 4 - e
    {
        gpio_set_level(GPIO_OUTPUT_IO_4, 0);
    }
    else
    {
        gpio_set_level(GPIO_OUTPUT_IO_4, 1);
    }

    if(lookup_table[digit] & 0x20) // bit 5 - f
    {
        gpio_set_level(GPIO_OUTPUT_IO_5, 0);
    }
    else
    {
        gpio_set_level(GPIO_OUTPUT_IO_5, 1);
    }

    if(lookup_table[digit] & 0x40) // bit 6 - g
    {
        gpio_set_level(GPIO_OUTPUT_IO_6, 0);
    }
    else
    {
        gpio_set_level(GPIO_OUTPUT_IO_6, 1);
    }
/**/
}

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
            digit = 0x10;
        }
    }
    else
    {
        cont = 0;
    }

    if(flag_off)
    {
        // Apagado
        digit = 0x10;
    }

    if(flag_TO10 == 1)
    {
        // guión
        digit = 0x20;
    }
    
    return print_digit(digit);

}