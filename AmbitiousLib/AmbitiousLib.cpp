#include "stdafx.h"
#include "magic.h"
#include "magicpid.h"
#include "decoder.h"
#include "checklic.h"
#include "AmbitiousLib.h"

namespace AmbitiousLib {

int KeyServices::ValidateKey(unsigned char key[], int productIndex, 
							 unsigned int* Mod, unsigned int* A1A2, unsigned int* B1B2,
							 unsigned int* UpperKey1, unsigned int* UpperKey2, unsigned int* UpperKey3, unsigned int* UpperKey4, 
							 unsigned int* LowerKey)
{
    static BYTE ClaveSup[16];
    static BYTE ClaveInf[4];
    int Resultado;
        
	DWORD Tabla[364];
	DWORD* TablaRef = Tabla;
    if(EscribeTablaMagica(TablaRef, Mod, A1A2, B1B2, 12, 0x1C, 0x37)!= 0)
        return ERROR_NUMMAGICOS;

    // Limpia bytes de relleno
    ClaveSup[11] = 0;
    // Convierte clave en magnitud
    if(ClaveMagnitud(key, ClaveSup, 11, ClaveInf) != -1)
	{
		*UpperKey1 = *((DWORD*)ClaveSup);
		*UpperKey2 = *(((DWORD*)ClaveSup)+1);
		*UpperKey3 = *(((DWORD*)ClaveSup)+2);
		*UpperKey4 = *(((DWORD*)ClaveSup)+3);
		*LowerKey = *((DWORD*)ClaveInf);
	}
	else
	{
		*UpperKey1 = 0;
		*UpperKey2 = 0;
		*UpperKey3 = 0;
		*UpperKey4 = 0;
		*LowerKey = 0;
	}

    Resultado = CheckLic(TablaRef, ClaveInf, 4, ClaveSup);

	return Resultado;
};

int KeyServices::SearchProductFromKey(unsigned char key[], 
							 unsigned int* UpperKey1, unsigned int* UpperKey2, unsigned int* UpperKey3, unsigned int* UpperKey4, 
							 unsigned int* LowerKey)
{
    static BYTE ClaveSup[16];
    static BYTE ClaveInf[4];
    int Resultado;
        
    // Limpia bytes de relleno
    ClaveSup[11] = 0;
    // Convierte clave en magnitud
    if(ClaveMagnitud(key, ClaveSup, 11, ClaveInf) != -1)
	{
		*UpperKey1 = *((DWORD*)ClaveSup);
		*UpperKey2 = *(((DWORD*)ClaveSup)+1);
		*UpperKey3 = *(((DWORD*)ClaveSup)+2);
		*UpperKey4 = *(((DWORD*)ClaveSup)+3);
		*LowerKey = *((DWORD*)ClaveInf);
	}
	else
	{
		*UpperKey1 = 0;
		*UpperKey2 = 0;
		*UpperKey3 = 0;
		*UpperKey4 = 0;
		*LowerKey = 0;
	}

	for(int i=1; i<NUM_PRODUCTOS; i++)
    {
	    Resultado = CheckLic(Numero2Producto(i), ClaveInf, 4, ClaveSup);
        switch(Resultado)
        {
            case OK:
				return i;
        }
    }

	return 0;
};


unsigned int* KeyServices::Index2Product(int productIndex)
{
	return Numero2Producto(productIndex);
}

int KeyServices::GeneratePID(unsigned int* PublicKey, unsigned int* GxGy, unsigned int* s1, unsigned int* s2)
{
    return GeneraNumerosMagicos(PublicKey, GxGy, s1, s2, 12) == ERROR_NUMMAGICOS;
}

int KeyServices::GenerateKey(unsigned char** key,
							 unsigned int* Mod, unsigned int* A1A2, unsigned int* B1B2,
							 unsigned int* LowerKey,
							 unsigned int* GxGy, unsigned int* s1, unsigned int* s2, unsigned int* fLow, unsigned int* fHigh,
							 unsigned int* UpperKey1, unsigned int* UpperKey2, unsigned int* UpperKey3, unsigned int* UpperKey4)
{
    static BYTE ClaveSup[16];
    static BYTE ClaveInf[4];
    static BYTE ClaveTexto[30];
    int Resultado;

	DWORD Tabla[364];
	DWORD* TablaRef = Tabla;
    if(EscribeTablaMagica(TablaRef, Mod, A1A2, B1B2, 12, 0x1C, 0x37)!= 0)
        return ERROR_NUMMAGICOS;

	*((unsigned int*)(ClaveInf)) = *LowerKey;
	ClaveInf[3] &= 0x7F;	// 31 bits only -just in case-

    DWORD s1pad[12], s2pad[12], f[12];
    for(int i=0; i<12; i++)
        s1pad[i] = s2pad[i] = f[i] = 0;
	s1pad[0] = *s1;
	s2pad[0] = *s2;

tryagain:;
    Resultado = GenLic(TablaRef, GxGy, s1pad, s2pad, f, ClaveInf, 4, ClaveSup);

    switch(Resultado)
    {
        case OK:
            ClaveSup[11] = 0;
            if(MagnitudClave(ClaveTexto, ClaveSup, 11, ClaveInf) == -1)
                goto tryagain;

			*UpperKey1 = *((DWORD*)ClaveSup);
			*UpperKey2 = *(((DWORD*)ClaveSup)+1);
			*UpperKey3 = *(((DWORD*)ClaveSup)+2);
			*UpperKey4 = *(((DWORD*)ClaveSup)+3);
			*LowerKey = *((DWORD*)ClaveInf);

			*s1 = s1pad[0];
			*s2 = s2pad[0];
			*fLow = *((DWORD*)f);
			*fHigh = *(((DWORD*)f)+1);

			*key = ClaveTexto;

			return OK;
            break;

		default:
			return Resultado;
	}
};
}