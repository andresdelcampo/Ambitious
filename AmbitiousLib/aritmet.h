/*****************************************************************************
MODULO: aritmet
DESCRIPCION: Aritmética en módulo para cualquier magnitud
*****************************************************************************/

#ifndef __ARITMET_H
#define __ARITMET_H


#define DWORD   unsigned int


void Copia(DWORD *A, DWORD *Res, int Tam);
int Compara(DWORD *A, DWORD *B, int Tam);
int Compara3(DWORD *A1A2, DWORD *B, int Tam);
int GetBit(DWORD *A, int NumBit);
void SetBit(DWORD *A, int NumBit, int Valor);
int Suma(DWORD *A, DWORD *B, DWORD *Res, int Tam);
int Resta(DWORD *A, DWORD *B, DWORD *Res, int Tam);
void SumaModulo(DWORD *A, DWORD *B, DWORD *Res, DWORD *Mod, int Tam);
void RestaModulo(DWORD *A, DWORD *B, DWORD *Res, DWORD *Mod, int Tam);
void MultiplicaModulo(DWORD *A, DWORD *B, DWORD *Res, DWORD *Mod, int Tam);
int DivisionPor2Modulo(DWORD *A, DWORD *Mod, int Tam);
void DivideModulo(DWORD *A, DWORD *B, DWORD *Res, DWORD *Mod, int Tam);


#endif
