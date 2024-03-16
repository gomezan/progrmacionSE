/* Buffer.c    Implementación del modulo del Buffer */

#include "Buffer.h"
#include "driver/gpio.h"

/* ======= Rutinas ======== */

/* Rutina para iniciar el módulo (su estructura de datos) */   
void Bf_Inicie(Buffer_Control *buf, Bf_data *datos,  Bf_pointer total)
{
    //Vector de datos
    buf->datos = datos;
    //Número de datos total
    buf->total = total;
    //
    buf->perc_25 = (buf->total) * 0.25;
    //Número de datos presentes
    buf->cont = 0;

    //Incializar datos
    for(;total;--total,++datos)
        *datos = 0;
    
    //Posiciones de lectura y escritura
    buf->read = 0;   //Posición de lectura 
    buf->write = 0;   //Posición de ecritura

    //Banderas de funcionamiento del buffer
    buf->full = 0;    //Buffer lleno
    buf->active = 1; //Activo o inactivo
}

/* Rutina para verificar si el buffer está o no lleno */
char Bf_Lleno(Buffer_Control *buf)
{
    if(!(buf->active))
        return -1;

    if(buf->full)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
    
}

/* Rutina para verificar si el buffer está vacío */
char Bf_Vacio(Buffer_Control *buf)
{
    if(!(buf->active))
        return -1;

    if(buf->cont == 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* Rutina para verificar si el buffer sobrepasa el 75% de capacidad */
char Bf_75_Lleno(Buffer_Control *buf)
{
    if(!(buf->active))
        return -1;

    if(buf->cont >= (buf->total - buf->perc_25))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* Rutina para verificar si el buffer está vacío en un 75% de su capacidad total */
char Bf_75_Vacio(Buffer_Control *buf)
{
    if(!(buf->active))
        return -1;

    if(buf->cont <= buf->perc_25)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* Rutina para subir un dato al buffer */
char Bf_Subir_Dato(Buffer_Control *buf, Bf_data write)
{
    if(!(buf->active))
        return FALSE;
    
    if(!(buf->full))
    {
        //Incrementar capacidad del buffer
        ++(buf->cont);
        //Se escribe el núevo dato en la  posición correcta
        (buf->datos)[buf->write] = write;
        //Se incrementa la posición si esta se encuentra dentro del arreglo
        if(buf->write < (buf->total-1))
        {
            ++(buf->write);
        }
        //Reiniciar posición
        else
        {
            buf->write = 0;
        }
        //Si las psociiones se encuentran entonces esta lleno el buffer
        if(buf->write == buf->read)
            buf->full = 1;

        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/* Rutina para bajar un dato del buffer */
char Bf_Bajar_Dato(Buffer_Control *buf, Bf_data *read)
{
    if(!(buf->active))
        return FALSE;

    Bf_pointer free = 0;

    // Se revisa si hay datos en el buffer
    // Como el contador lleva los datos adentro del buffer, si es mayor que 0, hay al menos un dato
    if(buf->cont > 0)
    {
        //Se direcciona el apuntador al valor siguiente
        (*read) = (buf->datos)[buf->read];
        //Se incrementa la posición si esta se encuentra dentro del arreglo
        if(buf->read < (buf->total-1))
        {
            ++(buf->read);
        }
        //Reiniciar posición
        else
        {
            buf->read = 0;
        }
        //Actualizar variables del buffer
        --(buf->cont);
        buf->full = 0;

        return TRUE;
    }
    return FALSE;
}

/* Rutina para verificar cuantos espacios tiene libres */
char Bf_Libre(Buffer_Control *buf, Bf_pointer *free)
{
    if(!(buf->active))
        return FALSE;
        
    //Devuelve total menos presentes
    (*free) = buf->total - buf->cont;
    
    return TRUE;
}