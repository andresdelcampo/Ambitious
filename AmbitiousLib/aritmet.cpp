/*****************************************************************************
MODULO: aritmet
DESCRIPCION: Aritmética en módulo para cualquier magnitud
*****************************************************************************/

#include "stdafx.h"
#include "aritmet.h"

/*****************************************************************************
FUNCION: Copia
DESCRIPCION: A (origen), Res (destino), Tam (tamaño en enteros)
*****************************************************************************/
void Copia(DWORD *A, DWORD *Res, int Tam)
{
    for(int i=0; i<Tam; i++)
        Res[i] = A[i];
}


/*****************************************************************************
FUNCION: Compara
DESCRIPCION: A y B (sumandos), Tam (tamaño en enteros). Devuelve -1,0,1
    dependiendo de cuál es más grande (B, =, A)
*****************************************************************************/
int Compara(DWORD *A, DWORD *B, int Tam)
{
    int Res = 0;

    for(int i=Tam-1; i>=0 && Res==0; i--)
    {
        if (A[i] > B[i])
            Res = 1;
        if (A[i] < B[i])
            Res = -1;
    }
    return Res;
}


/*****************************************************************************
FUNCION: Compara3
DESCRIPCION: A1A2 (dos números seguidos) y B (otro), Tam (tamaño en enteros).
    Devuelve 1 si son iguales. Si no, 0
*****************************************************************************/
int Compara3(DWORD *A1A2, DWORD *B, int Tam)
{
    DWORD *A1 = A1A2;
    DWORD *A2 = &A1A2[Tam];

    return (Compara(A1, A2, Tam)==0 && Compara(A1, B, Tam)==0);
}


/*****************************************************************************
FUNCION: GetBit
DESCRIPCION: Accede al bit NumBit-ésimo de A.
*****************************************************************************/
int GetBit(DWORD *A, int NumBit)
{
    DWORD Res = A[NumBit >> 5];
    Res >>= (NumBit % 32);
    return (Res & 1);
}


/*****************************************************************************
FUNCION: SetBit
DESCRIPCION: Fija el bit NumBit-ésimo de A al valor indicado.
*****************************************************************************/
void SetBit(DWORD *A, int NumBit, int Valor)
{
    DWORD Mask = 1 << (NumBit % 32);
    if(Valor)
        A[NumBit >> 5] |= Mask;
    else
        A[NumBit >> 5] &= ~Mask;
}


/*****************************************************************************
FUNCION: Suma
DESCRIPCION: A y B (sumandos), Res (resultado), Tam (tamaño en enteros)
*****************************************************************************/
int Suma(DWORD *A, DWORD *B, DWORD *Res, int Tam)
{
    int Acarreo = 0;
    DWORD Sum;

    for(int i=0; i<Tam; i++)
    {
        Sum = A[i] + B[i] + Acarreo;
        Acarreo = (Sum < A[i] || Sum < B[i])? 1 : 0;
        Res[i] = Sum;
    }
    return Acarreo;
}


/*****************************************************************************
FUNCION: Resta
DESCRIPCION: A y B (minuendo y sustraendo), Res (resultado), Tam (tamaño en int)
*****************************************************************************/
int Resta(DWORD *A, DWORD *B, DWORD *Res, int Tam)
{
    int Acarreo = 0;
    DWORD Rest;

    for(int i=0; i<Tam; i++)
    {
        Rest = A[i] - B[i] - Acarreo;
        Acarreo = (A[i] < B[i])? 1 : 0;
        Res[i] = Rest;
    }
    return Acarreo;
}


/*****************************************************************************
FUNCION: SumaModulo
DESCRIPCION: A y B (sumandos), Res (resultado), Mod (módulo), Tam (num.enteros)
*****************************************************************************/
void SumaModulo(DWORD *A, DWORD *B, DWORD *Res, DWORD *Mod, int Tam)
{
    int Acarreo;
    Acarreo = Suma(A, B, Res, Tam);
    if(Acarreo || Compara(Res, Mod, Tam) >= 0)
        Resta(Res, Mod, Res, Tam);
}


/*****************************************************************************
FUNCION: RestaModulo
DESCRIPCION: A y B (valores), Res (resultado), Mod (módulo), Tam (num.enteros)
*****************************************************************************/
void RestaModulo(DWORD *A, DWORD *B, DWORD *Res, DWORD *Mod, int Tam)
{
    if(Resta(A, B, Res, Tam))
        Suma(Res, Mod, Res, Tam);
}


/*****************************************************************************
FUNCION: MultiplicaModulo
DESCRIPCION: (A * B) % Mod
*****************************************************************************/
void MultiplicaModulo(DWORD *A, DWORD *B, DWORD *Res, DWORD *Mod, int Tam)
{
    for(int i=0; i<Tam; i++)
        Res[i] = 0;

    // Recorre todos los bits del MSB al LSB
    for(int i=32*Tam-1; i>=0; i--)
    {
        SumaModulo(Res, Res, Res, Mod, Tam);
        if(GetBit(A, i))
            SumaModulo(Res, B, Res, Mod, Tam);
    }
}


/*****************************************************************************
FUNCION: DivisionPor2Modulo
DESCRIPCION: Inversa de multiplicación por 2.
*****************************************************************************/
int DivisionPor2Modulo(DWORD *A, DWORD *Mod, int Tam)
{
    int Acarreo, AcarreoAnt = 0;
    // Si el número es impar, suma el módulo
    if(A[0] & 1)
        AcarreoAnt = Suma(A, Mod, A, Tam);
    for(int i=Tam-1; i>=0; i--)
    {
        Acarreo = A[i] & 1;
        A[i] >>= 1;
        // Si hubo acarreo pone el MSB a 1
        if(AcarreoAnt)
            A[i] = A[i] | 0x80000000;
        AcarreoAnt = Acarreo;
    }
    return Acarreo;
}


/*****************************************************************************
FUNCION: DivideModulo
DESCRIPCION: Inversa a MultiplicaModulo.
*****************************************************************************/
void DivideModulo(DWORD *A, DWORD *B, DWORD *Res, DWORD *Mod, int Tam)
{
    bool terminar = false;
    DWORD *Temp = new DWORD[Tam*4];
    DWORD *Ta = Temp;
    DWORD *Tb = &Temp[Tam];
    DWORD *Tc = &Temp[Tam*2];
    DWORD *Td = &Temp[Tam*3];

    // Inicializa los 4 campos necesarios para el algoritmo
    Copia(A, Ta, Tam);
    for(int i=0; i<Tam; i++)
        Tb[i] = 0;
    Copia(B, Tc, Tam);
    Copia(Mod, Td, Tam);

    do
    {
        if(!(Tc[0] & 1))    // si C es par
        {
            DivisionPor2Modulo(Tc, Mod, Tam);
            DivisionPor2Modulo(Ta, Mod, Tam);
        }
        else if(!(Td[0] & 1)) // si D es par
        {
            DivisionPor2Modulo(Td, Mod, Tam);
            DivisionPor2Modulo(Tb, Mod, Tam);
        }
        else if(Compara(Tc, Td, Tam)==0)
            terminar = true;
        else
        {
            if(Compara(Tc, Td, Tam) > 0)
            {
                Resta(Tc, Td, Tc, Tam);
                RestaModulo(Ta, Tb, Ta, Mod, Tam);
            }
            else
            {
                Resta(Td, Tc, Td, Tam);
                RestaModulo(Tb, Ta, Tb, Mod, Tam);
            }
        }
    } while (!terminar);

    Copia(Ta, Res, Tam);
    delete [] Temp;
}


