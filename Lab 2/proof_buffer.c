#include <stdio.h>
#include "Buffer.h"

#define SIZE_BUFFER 64

Buffer_Control c_buffer;

Bf_data data[SIZE_BUFFER] = {};

int main()
{
    Bf_data write = 23;
    Bf_data read = 2;
    Bf_pointer free = 0;

    Bf_Inicie(&c_buffer,data,SIZE_BUFFER);

    int menu = 1;

    while(menu>=0)
    {
        printf("\n\n\nIngrese el valor de menu: ");
        scanf("%d",&menu);

        printf("El valor ingresado fue %d\n",menu);
        if(menu == 0)
        {
            printf("El buffer esta lleno %d\n",Bf_Lleno(&c_buffer));
        }
        else if(menu == 1)
        {   
            printf("El dato a subir es %d\n",write);
            printf("El buffer pudo subir dato %d\n",Bf_Subir_Dato(&c_buffer,write)); 
            printf("Ingrese un nuevo valor para enviar la siguiente vez: ");
            scanf("%d",&write);
        }
        else if(menu == 2)
        {
            printf("El dato anterior es: %d\n",read);
            printf("El buffer pudo bajar dato %d\n",Bf_Bajar_Dato(&c_buffer,&read));
            printf("El dato bajado fue: %d\n",read);
        }
        else if(menu == 3)
        {
            printf("Se pudo obtener el tamaño del buffer %d\n",Bf_Libre(&c_buffer,&free));
            printf("El espacio libre es: %d\n",free);
        }
    }

    return 0;
}