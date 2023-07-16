/*****************************************************************************
MODULO: checklic
DESCRIPCION: Funciones de comprobación de licencia Micro$oft
*****************************************************************************/


#ifndef __CHECKLIC_H
#define __CHECKLIC_H

#define OK                  0
#define MAL                 1
#define ERROR_NUMMAGICOS    3
#define ERROR_OTROF         4

#define DWORD   unsigned int
#define BYTE    unsigned char

int CheckLic(DWORD *NumMagicos, BYTE *ClaveInf, int TamClave, BYTE *ClaveSup);
int GenLic(DWORD *NumMagicos, DWORD *G1G2, DWORD *s1, DWORD *s2, DWORD *f,
            BYTE *ClaveInf, int TamClave, BYTE *ClaveSup);
int GetCharBit(char *A, int NumBit);
void SetCharBit(char *A, int NumBit, int Valor);
void MultiplicaEnCurvaEliptica(DWORD *A1A2, DWORD *N, int NumBits, DWORD *R1R2,
                         DWORD *UnoCero, DWORD *Mod, int Tam);
void SumaEnCurvaEliptica(DWORD *A1A2, DWORD *B1B2, DWORD *C1C2, DWORD *UnoCero,
                      DWORD *Mod, int Tam);
void Inicializa_SHA1(DWORD *mem);
void SHA1_Invertido(DWORD *Res, DWORD *Dato);
void PreparaFragmento(DWORD *Estado, BYTE *Fragmento, int TamB);
void PreparaUltimoFragmento(DWORD *Estado, DWORD *Res);
void ProcesaSHA1(DWORD *ClaveInf, int TamClave, DWORD *N, int Tam,
                     DWORD *R, int TamR);
                     
#endif
