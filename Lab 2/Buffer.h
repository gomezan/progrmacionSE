/* Buffer.h    Definiciones del módulo Buffer */
#if !defined(BUFFER_H)

#define  BUFFER_H

typedef unsigned int Bf_data;

typedef unsigned char Bf_pointer;

typedef struct Buffer_Control Buffer_Control;

struct Buffer_Control
   {

   /* Banderas */
   unsigned char  full;  // 1 si está lleno y 0 si no
   unsigned char active; // 1 si el buffer está activo y 0 si no
                  
   /* Datos */
   Bf_data *datos;
   
   /* Control del tamaño del buffer */
   Bf_pointer read,  // Posición en buffer de lectura
              write, // Posición en buffer de escritura
              cont,  // Valores guardados en un instante
              total; // Total de valores a guardar
   };


/* ======= Rutinas ======== */

/* Rutina para iniciar el módulo (su estructura de datos) */   
void Bf_Inicie(Buffer_Control *buf, Bf_data *datos, Bf_pointer total);

/* Rutina para verificar si el buffer está o no lleno */
char Bf_Lleno(Buffer_Control *buf);

/* Rutina para subir un dato al buffer */
char Bf_Subir_Dato(Buffer_Control *buf, Bf_data write);

/* Rutina para bajar un dato del buffer */
char Bf_Bajar_Dato(Buffer_Control *buf, Bf_data *read);

/* Rutina para verificar cuantos espacios tiene libres */
char Bf_Libre(Buffer_Control *buf, Bf_pointer *free);

#endif