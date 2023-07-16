/*****************************************************************************
MODULO: magic
DESCRIPCION: Funciones de números mágicos
*****************************************************************************/

#ifndef __MAGIC_H
#define __MAGIC_H

#define DWORD   unsigned int
#define BYTE    unsigned char

// El error de checksum lo he definido yo diferente porque algunos productos
// no lo cumplen
#define ERROR_CHECKSUM    4
#define ERROR_NUMMAGICOS  3


int CompruebaEnCurvaEliptica(DWORD *A1A2, DWORD *UnoCero, DWORD *Mod, int Tam);
int LeeTablaMagica(DWORD *Tabla, DWORD **Mod, DWORD **UnoCero, DWORD **A1A2,
                   DWORD **B1B2, int *Tam, int *TamCSup1, int *TamCSup2);
int EscribeTablaMagica(DWORD *Res, DWORD *Mod, DWORD *A1A2, DWORD *B1B2,
                        int Tam, int TamCSup1, int TamCSup2);
int GeneraNumerosMagicos(DWORD *Tabla, DWORD *G1G2, DWORD *s1, DWORD *s2,
                         int Tam);
#endif