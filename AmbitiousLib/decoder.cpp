/*****************************************************************************
MODULO: decoder
DESCRIPCION: Funciones de codificación/decodificación entre clave y magnitud
*****************************************************************************/

#include "stdafx.h"
#include "decoder.h"
#include <stdio.h>


/*****************************************************************************
FUNCION: ClaveMagnitud
DESCRIPCION: Transforma cadena de caracteres (base 24) a magnitud (base 16)
    en dos fragmentos (el inferior de 31 bits y el superior lo demás).
*****************************************************************************/
int ClaveMagnitud(BYTE *Clave, BYTE *MagSup, int TamSup, BYTE *MagInf)
{
    char alfabeto[] = "BCDFGHJKMPQRTVWXY2346789";
    BYTE Temp[68];
    BYTE *ptr;
    DWORD Actual;
    int i,j;
    int MaxBytes = 0;

    // Limpia resultados
    for(i=0; i<68; i++)
        Temp[i] = 0;

    for(i=0, ptr=Clave; i<25; i++, ptr++)
    {
        // Salta los guiones
        if(i%5==0 && i!=0)
            ptr++;
        // Localiza la letra
        Actual = -1;
        for(j=0; j<24; j++)
            if(*ptr == alfabeto[j])
            {
                Actual = j;
                break;
            }
        // Letra no válida
        if(Actual == -1)
            return -1;
        // Multiplica...
        for(j=0; j<MaxBytes; j++)
        {
            Actual += (DWORD)Temp[j] * 24;
            Temp[j] = Actual & 0xFF;
            Actual >>= 8;
        }
        if(Actual > 0)
        {
            Temp[j] = (BYTE)Actual;
            MaxBytes = j+1;
        }
    }

    // Copia los 31 bits más bajos a la magnitud inferior
    MagInf[0] = Temp[0];
    MagInf[1] = Temp[1];
    MagInf[2] = Temp[2];
    MagInf[3] = Temp[3] & 0x7F;

    // Recoge lo demás contando con que hay que desplazar todo un bit
    for(i=0; i<TamSup; i++)
        MagSup[i] = ((Temp[i+3] >> 7) & 1)  |  (Temp[i+4] << 1);

    return 0;
}


/*****************************************************************************
FUNCION: MagnitudClave
DESCRIPCION: Genera una clave a partir de una magnitud
*****************************************************************************/
int MagnitudClave(BYTE *Clave, BYTE *MagSup, int TamSup, BYTE *MagInf)
{
    char alfabeto[] = "BCDFGHJKMPQRTVWXY2346789";
    BYTE Temp[68];
    BYTE *ptr;
    DWORD Actual;
    int i,j;
    int MaxBytes = TamSup + 4;

    // Limpia resultados
    for(i=0; i<68; i++)
        Temp[i] = 0;

    // Copia los 31 bits más bajos a la magnitud inferior
    Temp[0] = MagInf[0];
    Temp[1] = MagInf[1];
    Temp[2] = MagInf[2];
    Temp[3] = MagInf[3] & 0x7F;

    // Recoge lo demás contando con que hay que desplazar todo un bit
    for(i=0; i<TamSup; i++)
    {
        Temp[i+3] = Temp[i+3] | ((MagSup[i] & 1) << 7);
        Temp[i+4] = MagSup[i] >> 1;
    }

    Clave[29] = '\0';
    for(i=24, ptr=Clave+28; i>=0; i--, ptr--)
    {
        // Salta los guiones
        if(i%5==4 && i!=24)
        {
            *ptr = '-';
            ptr--;
        }

        // Divide por 24
        Actual = 0;
        for(j=MaxBytes-1; j>=0; j--)
        {
            int Tmp;
            Actual <<= 8;
            Actual += (DWORD)Temp[j];
            Tmp = Actual % 24;
            Temp[j] = ((Actual-Tmp) / 24) & 0xFF;
            Actual = Actual % 24;
            // Ya quedan menos bytes por calcular
            if(j==MaxBytes-1 && Temp[j]==0)
                MaxBytes--;
        }
        // Por algún extraño motivo acaba en -1
        *ptr = alfabeto[Actual];
    }

    // No debe haber sobrado nada
    if(MaxBytes)
        return -1;
    return 0;
}
