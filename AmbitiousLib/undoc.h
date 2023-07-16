/*****************************************************************************
 MODULO: undoc
 DESCRIPCION: Funciones indocumentadas de pidgen.dll
*****************************************************************************/

#ifndef __UNDOC_H
#define __UNDOC_H

#define DWORD   unsigned int
#define BYTE    unsigned char


void Monstruo2(DWORD *res, DWORD *mem);
void Generador(BYTE *tabla, BYTE *llave, int longllave);
void Modificador(BYTE *tabla, BYTE *llave, int longllave);


#endif