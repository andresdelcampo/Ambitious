/*****************************************************************************
MODULO: magic
DESCRIPCION: Funciones de números mágicos
*****************************************************************************/

#include "stdafx.h"
#include "aritmet.h"
#include "magic.h"
#include "undoc.h"
#include "checklic.h"
#include <stdlib.h>
#include <time.h>
#include "magicpid.h"

#define TAM_CABECERA    7
#define TAM_MAX         32
#define CSUP1_MAX       32
#define CSUP2_MAX       64
#define NUM_MAGICOS     7
#define MAGIC_CODE      0x0130DFAE


/*****************************************************************************
FUNCION: CompruebaEnCurvaEliptica
DESCRIPCION: Comprueba que los números mágicos cumplen la propiedad...
*****************************************************************************/
int CompruebaEnCurvaEliptica(DWORD *A1A2, DWORD *UnoCero, DWORD *Mod, int Tam)
{
    DWORD *A1 = A1A2;
    DWORD *A2 = &A1A2[Tam];
    DWORD *Uno = UnoCero;
    DWORD *Cero = &UnoCero[Tam];
    DWORD *R1 = new DWORD[Tam*3];
    DWORD *R2 = &R1[Tam];
    DWORD *R3 = &R2[Tam];
    int Res;

    if(!R1) return 0;

    if(Compara3(A1A2, Cero, Tam))
        return 1;

    /* (A1*A1 + B1)*A1 + B2 */
    MultiplicaModulo(A1, A1, R1, Mod, Tam);
    SumaModulo(R1, Uno, R1, Mod, Tam);
    MultiplicaModulo(R1, A1, R2, Mod, Tam);
    SumaModulo(R2, Cero, R2, Mod, Tam);

    /* A2*A2 */
    MultiplicaModulo(A2, A2, R3, Mod, Tam);

    Res = Compara(R2, R3, Tam);
    delete [] R1;
    if(Res == 0)
        return 1;
    else
        return 0;
}


/*****************************************************************************
FUNCION: LeeTablaMagica
DESCRIPCION: Lee la estructura de números mágicos
*****************************************************************************/
int LeeTablaMagica(DWORD *Tabla, DWORD **Mod, DWORD **UnoCero, DWORD **A1A2,
                   DWORD **B1B2, int *Tam, int *TamCSup1, int *TamCSup2)
{
    DWORD Checksum = 0;
    int Resultado;

    // Múltiples causas de error en cabecera
    if(Tabla[0] <= TAM_CABECERA ||
       Tabla[1] != NUM_MAGICOS ||
       Tabla[3] != MAGIC_CODE ||
       Tabla[4] > TAM_MAX ||
       Tabla[5] > CSUP1_MAX ||
       Tabla[6] > CSUP2_MAX ||
       Tabla[0] != 4*(Tabla[1]*Tabla[4] + TAM_CABECERA))
       return ERROR_NUMMAGICOS;

    // Calcula y fija checksum
    Resultado = 0;
    for(unsigned int i=0; i<(Tabla[0] / 4); i++)
        Checksum += Tabla[i];
    if(Checksum != 0)
        Resultado = ERROR_CHECKSUM;

    // Guarda los tamaños
    if(Tam)
        *Tam = Tabla[4];
    if(TamCSup1)
        *TamCSup1 = Tabla[5];
    if(TamCSup2)
        *TamCSup2 = Tabla[6];

    // Guarda las direcciones
    if(Mod)
        *Mod = &Tabla[TAM_CABECERA];
    if(UnoCero)
        *UnoCero = *Mod + Tabla[4];
    if(A1A2)
        *A1A2 = *UnoCero + Tabla[4]*2;
    if(B1B2)
        *B1B2 = *A1A2 + Tabla[4]*2;

    return Resultado;
}


/*****************************************************************************
FUNCION: EscribeTablaMagica
DESCRIPCION: Crea la estructura a partir de los números.
*****************************************************************************/
int EscribeTablaMagica(DWORD *Res, DWORD *Mod, DWORD *A1A2, DWORD *B1B2,
                        int Tam, int TamCSup1, int TamCSup2)
{
    DWORD Checksum = 0;

    // Múltiples causas de error en cabecera
    if(Tam > TAM_MAX || TamCSup1 > CSUP1_MAX || TamCSup2 > CSUP2_MAX ||
       !Res || !Mod || !A1A2 || !B1B2)
       return ERROR_NUMMAGICOS;

    // Tamaño de la tabla (7 ints la cabecera + 7 números mágicos)
    Res[0] = (TAM_CABECERA + NUM_MAGICOS*Tam) * 4;
    Res[1] = NUM_MAGICOS;   // Cantidad de números mágicos, Siempre 7
    Res[2] = 0;             // Checksum. En principio, 0
    // Magic number que indica que esto es una tabla de números mágicos
    Res[3] = MAGIC_CODE;
    Res[4] = Tam;           // Tamaño por número
    // Bits en los que dividir la magnitud superior (así se queda de momento)
    Res[5] = TamCSup1;
    Res[6] = TamCSup2;

    // Copia el módulo
    Copia(Mod, &Res[TAM_CABECERA], Tam);
    // Crea el uno y el cero... (primer byte 1 y el resto de los 2 números a 0)
    Res[7+Tam] = 1;
    for(int i=TAM_CABECERA+Tam+1; i< TAM_CABECERA+Tam*3; i++)
        Res[i] = 0;
    // Copia los números A1A2, B1B2
    Copia(A1A2, &Res[ TAM_CABECERA + 3*Tam ], Tam*2);
    Copia(B1B2, &Res[ TAM_CABECERA + 5*Tam ], Tam*2);

    // Calcula y fija checksum
    for(int i=0; i<TAM_CABECERA+NUM_MAGICOS*Tam; i++)
        Checksum += Res[i];
    Res[2] = 0 - Checksum;

    return 0;
}


/*****************************************************************************
FUNCION: GeneraNumerosMagicos
DESCRIPCION:
*****************************************************************************/
int GeneraNumerosMagicos(DWORD *Tabla, DWORD *G1G2, DWORD *s1, DWORD *s2, int Tam)
{
    DWORD Mod[TAM_MAX];
    DWORD A1A2[TAM_MAX*2];
    DWORD B1B2[TAM_MAX*2];
    DWORD Temp[TAM_MAX*2];
    DWORD UnoCero[TAM_MAX*2];
    DWORD *Producto;
    DWORD Aleatorio;
    static bool Randomized = false;

    if(!Randomized)
    {
        srand((unsigned) time(NULL));
        Randomized = true;
    }

    // Selecciona producto al azar
    Producto = Numero2Producto((rand() % NUM_PRODUCTOS) + 1);

    // Se copia el módulo del producto (todavía no he implementado generador
    // de primos) :p
    Copia(&Producto[TAM_CABECERA], Mod, Tam);

    // Genera valores 1 y 0.
    UnoCero[0] = 1;
    for(int i=1; i<TAM_MAX*2; i++)
        UnoCero[i] = 0;

    // Prepara un generador a partir de A ó B
    if(rand() % 2)
        Copia(&Producto[TAM_CABECERA + Tam*3], Temp, Tam*2);
    else
        Copia(&Producto[TAM_CABECERA + Tam*5], Temp, Tam*2);
    // ...al que multiplica unas cuantas veces...
    Aleatorio = rand();
    MultiplicaEnCurvaEliptica(Temp, &Aleatorio, 32, G1G2, UnoCero, Mod, Tam);
    if(!CompruebaEnCurvaEliptica(G1G2, UnoCero, Mod, Tam))
        return -1;

    // Si no hay valores de entrada... al azar!
    if(*s1 == 0)
    {
        *s1 = 1;    // s1, una potencia de 2 entre 1 y 256
        int j = (rand() %8) + 1;
        for(int i=0; i<j; i++)
            *s1 *= 2;
    }
    if(*s2 == 0)    // s2 cualquier número no 0 ni mayor de 2^26
        *s2 = rand();

    // Crea los A y B definitivos y escribe la tabla
    MultiplicaEnCurvaEliptica(G1G2, s1, 32, A1A2, UnoCero, Mod, Tam);
    MultiplicaEnCurvaEliptica(G1G2, s2, 32, B1B2, UnoCero, Mod, Tam);
    EscribeTablaMagica(Tabla, Mod, A1A2, B1B2, Tam, 0x1c, 0x37);

    return 0;
}



