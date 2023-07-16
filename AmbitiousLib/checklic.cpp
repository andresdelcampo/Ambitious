/*****************************************************************************
MODULO: checklic
DESCRIPCION: Funciones de comprobación de licencia Micro$oft
*****************************************************************************/

#include "stdafx.h"
#include "aritmet.h"
#include "magic.h"
#include "checklic.h"
#include "undoc.h"
#include <stdlib.h>

#define ROL _lrotl
#define ROR _lrotr


/*****************************************************************************
FUNCION: SumaEnCurvaEliptica
DESCRIPCION: Suma los puntos A1A2 y B1B2 dentro de la curva elíptica UnoCero
    (valores A y B de la curva). Mod = p
*****************************************************************************/
void SumaEnCurvaEliptica(DWORD *A1A2, DWORD *B1B2, DWORD *C1C2, DWORD *UnoCero,
                  DWORD *Mod, int Tam)
{
    DWORD *A1 = A1A2;
    DWORD *A2 = &A1A2[Tam];
    DWORD *B1 = B1B2;
    DWORD *B2 = &B1B2[Tam];
    DWORD *C1 = C1C2;
    DWORD *C2 = &C1C2[Tam];
    DWORD *Uno = UnoCero;
    DWORD *Cero = &UnoCero[Tam];
    DWORD *R1 = new DWORD[Tam*3];
    DWORD *R2 = &R1[Tam];
    DWORD *R3 = &R2[Tam];

    // A1==Mod y A2==Mod
    if(Compara(A1, Mod, Tam)==0 && Compara(A2, Mod, Tam)==0)
    {
        Copia(B1, C1, Tam);
        Copia(B2, C2, Tam);
        delete [] R1;
        return;
    }
    // B1==Mod y B2==Mod
    if(Compara(B1, Mod, Tam)==0 && Compara(B2, Mod, Tam)==0)
    {
        Copia(A1, C1, Tam);
        Copia(A2, C2, Tam);
        delete [] R1;
        return;
    }
    // A1==B1
    if(Compara(A1, B1, Tam) == 0)
    {
        SumaModulo(A2, B2, R2, Mod, Tam);
        if(Compara(R2, Cero, Tam) == 0)
        {
            Copia(Mod, C1, Tam);
            Copia(Mod, C2, Tam);
            delete [] R1;
            return;
        }
        MultiplicaModulo(A1, A1, R1, Mod, Tam);
        SumaModulo(R1, Uno, R3, Mod, Tam);
        SumaModulo(R3, R1, R3, Mod, Tam);
        SumaModulo(R3, R1, R3, Mod, Tam);
    }
    else
    {
        RestaModulo(A2, B2, R3, Mod, Tam);
        RestaModulo(A1, B1, R2, Mod, Tam);
    }
    DivideModulo(R3, R2, R1, Mod, Tam);
    MultiplicaModulo(R1, R1, R2, Mod, Tam);
    RestaModulo(R2, A1, R2, Mod, Tam);
    RestaModulo(R2, B1, R2, Mod, Tam);
    RestaModulo(A1, R2, R3, Mod, Tam);
    Copia(R2, C1, Tam);
    MultiplicaModulo(R1, R3, R2, Mod, Tam);
    RestaModulo(R2, A2, C2, Mod, Tam);
    delete [] R1;
}


/*****************************************************************************
FUNCION: MultiplicaEnCurvaEliptica
DESCRIPCION:
    A1A2: Punto de curva elíptica
    N: Escalar a multiplicar
    NumBits: número de bits de N
    R1R2: Resultado
    UnoCero: Valores A y B de la curva (este caso, 1 y 0)
    Mod: Valor P del campo de la curva elíptica
    Tam: Tamaño en dwords de A1A2 y Mod.
*****************************************************************************/
void MultiplicaEnCurvaEliptica(DWORD *A1A2, DWORD *N, int NumBits, DWORD *R1R2,
                                 DWORD *UnoCero, DWORD *Mod, int Tam)
{
    DWORD *R1 = R1R2;
    DWORD *R2 = &R1R2[Tam];

    // Inicializa resultados al módulo
    for(int i=0; i<Tam; i++)
    {
        R1[i] = Mod[i];
        R2[i] = Mod[i];
    }

    for(int i=NumBits-1; i>=0; i--)
    {
        SumaEnCurvaEliptica(R1R2, R1R2, R1R2, UnoCero, Mod, Tam);
        if(GetBit(N, i))
            SumaEnCurvaEliptica(R1R2, A1A2, R1R2, UnoCero, Mod, Tam);
    }
}


/*****************************************************************************
FUNCION: Inicializa_SHA1
*****************************************************************************/
void Inicializa_SHA1(DWORD *mem)
{
    mem[0] = 0x67452301;
    mem[1] = 0xEFCDAB89;
    mem[2] = 0x98BADCFE;
    mem[3] = 0x10325476;
    mem[4] = 0xC3D2E1F0;
    mem[5] = 0;
    mem[6] = 0;
}


/*****************************************************************************
FUNCION: SHA1_Inversion
PARAMETROS:
    SHA-1 modificado (realiza una inversión previa). Por lo demás, es idéntico
    a la especificación.
    res = estado que será modificado (5 enteros)
    dato = 16 enteros con dato origen (sólo se lee)
*****************************************************************************/
void SHA1_Inversion(DWORD *Res, DWORD *Dato)
{
    static DWORD mem[80];    /* 320 bytes */
    int i;
    DWORD _0_, _1_, _2_, _3_, _4_;

    /* Recorre los 64 bytes del bloque invirtiendo cada dword (16 int) */
    for(i=0; i<16; i++)
        mem[i] = ROL (Dato[i] & 0xFF00FF00,8)  ^  ROR (Dato[i]&0x00FF00FF,8);

    /* XORea un poco... creando una estructura 5 veces más grande (16+64 int)*/
    for(i=0; i<64; i+=2)
    {
        mem[i+16] = ROL(mem[i]   ^ mem[i+2] ^ mem[i+8] ^ mem[i+13] ,1);
        mem[i+17] = ROL(mem[i+1] ^ mem[i+3] ^ mem[i+9] ^ mem[i+14] ,1);
    }

    /* Se prepara... */
    _0_ = Res[0];
    _1_ = Res[1];
    _2_ = Res[2];
    _3_ = Res[3];
    _4_ = Res[4];


    /*** FASE A ***/
    for(i=0; i<20; i+=5)
    {
        _4_ += mem[i+0] + ROL(_0_, 5) + 0x5A827999 + (((_2_ ^ _3_) & _1_) ^ _3_);
        _1_ = ROR(_1_, 2);

        _3_ += mem[i+1] + ROL(_4_, 5) + 0x5A827999 + (((_1_ ^ _2_) & _0_) ^ _2_);
        _0_ = ROR(_0_, 2);

        _2_ += mem[i+2] + ROL(_3_, 5) + 0x5A827999 + (((_0_ ^ _1_) & _4_) ^ _1_);
        _4_ = ROR(_4_, 2);

        _1_ += mem[i+3] + ROL(_2_, 5) + 0x5A827999 + (((_4_ ^ _0_) & _3_) ^ _0_);
        _3_ = ROR(_3_, 2);

        _0_ += mem[i+4] + ROL(_1_, 5) + 0x5A827999 + (((_3_ ^ _4_) & _2_) ^ _4_);
        _2_ = ROR(_2_, 2);
    }

    /*** FASE B ***/
    for(i=20; i<40; i+=5)
    {
        _4_ += mem[i+0] + ROL(_0_, 5) + 0x6ED9EBA1 + (_2_ ^ _1_ ^ _3_);
        _1_ = ROR(_1_, 2);

        _3_ += mem[i+1] + ROL(_4_, 5) + 0x6ED9EBA1 + (_1_ ^ _0_ ^ _2_);
        _0_ = ROR(_0_, 2);

        _2_ += mem[i+2] + ROL(_3_, 5) + 0x6ED9EBA1 + (_0_ ^ _4_ ^ _1_);
        _4_ = ROR(_4_, 2);

        _1_ += mem[i+3] + ROL(_2_, 5) + 0x6ED9EBA1 + (_4_ ^ _3_ ^ _0_);
        _3_ = ROR(_3_, 2);

        _0_ += mem[i+4] + ROL(_1_, 5) + 0x6ED9EBA1 + (_3_ ^ _2_ ^ _4_);
        _2_ = ROR(_2_, 2);
    }

    /*** FASE C ***/
    for(i=40; i<60; i+=5)
    {
        _4_ += mem[i+0] + ROL(_0_, 5) + 0x8F1BBCDC + ((_1_ & _2_) | ((_1_ | _2_) & _3_));
        _1_ = ROR(_1_, 2);

        _3_ += mem[i+1] + ROL(_4_, 5) + 0x8F1BBCDC + ((_0_ & _1_) | ((_0_ | _1_) & _2_));
        _0_ = ROR(_0_, 2);

        _2_ += mem[i+2] + ROL(_3_, 5) + 0x8F1BBCDC + ((_4_ & _0_) | ((_4_ | _0_) & _1_));
        _4_ = ROR(_4_, 2);

        _1_ += mem[i+3] + ROL(_2_, 5) + 0x8F1BBCDC + ((_3_ & _4_) | ((_3_ | _4_) & _0_));
        _3_ = ROR(_3_, 2);

        _0_ += mem[i+4] + ROL(_1_, 5) + 0x8F1BBCDC + ((_2_ & _3_) | ((_2_ | _3_) & _4_));
        _2_ = ROR(_2_, 2);
    }

    /*** FASE D. Igual a B pero con otra constante ***/
    for(i=60; i<80; i+=5)
    {
        _4_ += mem[i+0] + ROL(_0_, 5) + 0xCA62C1D6 + (_2_ ^ _1_ ^ _3_);
        _1_ = ROR(_1_, 2);

        _3_ += mem[i+1] + ROL(_4_, 5) + 0xCA62C1D6 + (_1_ ^ _0_ ^ _2_);
        _0_ = ROR(_0_, 2);

        _2_ += mem[i+2] + ROL(_3_, 5) + 0xCA62C1D6 + (_0_ ^ _4_ ^ _1_);
        _4_ = ROR(_4_, 2);

        _1_ += mem[i+3] + ROL(_2_, 5) + 0xCA62C1D6 + (_4_ ^ _3_ ^ _0_);
        _3_ = ROR(_3_, 2);

        _0_ += mem[i+4] + ROL(_1_, 5) + 0xCA62C1D6 + (_3_ ^ _2_ ^ _4_);
        _2_ = ROR(_2_, 2);
    }

    /* Finaliza */
    Res[0] += _0_;
    Res[1] += _1_;
    Res[2] += _2_;
    Res[3] += _3_;
    Res[4] += _4_;
}


/*****************************************************************************
FUNCION: PreparaFragmento
DESCRIPCION: Va agrupando los pequeños paquetes hasta llenar 64 bytes, cuando
    llama a SHA1 para procesarlos.
*****************************************************************************/
void PreparaFragmento(DWORD *Estado, BYTE *Fragmento, int TamB)
{
    static BYTE Buffer[64];
    static int Pos = 0;

    if(Pos+TamB < 64)
    {
        for(int i=0; i<TamB; i++)
            Buffer[Pos+i] = Fragmento[i];
        Pos += TamB;
    }
    else
    {
        for(int i=0; i<64-Pos; i++)
            Buffer[Pos+i] = Fragmento[i];
        Pos = 0;
        SHA1_Inversion(Estado, (DWORD*)Buffer);
    }
}


/*****************************************************************************
FUNCION: PreparaUltimoFragmento
DESCRIPCION: Añade lo exigido por el protocolo SHA1: un bit a 1, una cadena de
    ceros y por último la longitud del mensaje. Lo envía invertido porque
    se ha modificado SHA1 para invertir al comienzo
*****************************************************************************/
void PreparaUltimoFragmento(DWORD *Estado, DWORD *Res)
{
    DWORD Temporal[80];
    int CTE_Copiado = 100;
    int Copiado = CTE_Copiado % 64;
    int PorCopiar = 64 - Copiado;
    // En el algoritmo original siempre será 0x1C
    if(PorCopiar <= 8)
        PorCopiar += 64;
    // Limpia, más vale que sobre
    for(int i=0; i<80; i++)
        Temporal[i] = 0;
    Temporal[0] = 0x00000080;       // 0 negativo invertido?
    Temporal[5] = 0x00000000;       // Dword del estado invertido
                                    // EstadoBytes[2C] << 3  |  CTE >> 0x1D
    Temporal[6] = 0x20030000;       // 320 (8*CTE) invertido

    PreparaFragmento(Estado, (BYTE*)Temporal, PorCopiar);

    // Copia estado invirtiendo
    for(int i=0; i<5; i++)
        Res[i] = ROL (Estado[i] & 0xFF00FF00,8)  ^  ROR (Estado[i]&0x00FF00FF,8);
}


/*****************************************************************************
FUNCION: ProcesaSHA1
DESCRIPCION: Algoritmo de DSS SHA1 para la firma digital de mensajes. No es
    reversible obviamente. Le envía los 31 bits inferiores de la clave seguidos
    del resultado obtenido por curvas elípticas
*****************************************************************************/
void ProcesaSHA1(DWORD *ClaveInf, int TamClave, DWORD *N, int Tam,
                     DWORD *R, int TamR)
{
    // Suma el tamaño de la clave inferior al tamaño del resultado (bits)
    // y convierte en dwords
    int TamRW = (TamR + 31) >> 5;
    int Despl = (TamRW << 5) - TamR;
    DWORD Estado[7];
    DWORD Temporal[16];

    Inicializa_SHA1(Estado);
    PreparaFragmento(Estado, (BYTE*)ClaveInf, TamClave);

    for(int i=0, j=0; i<Tam; i++, j++)
    {
        Temporal[j] = N[i];
        // Cada 20 bytes o si es el último de todos, lo añade al bloque
        if(j+1 > 4 || i==Tam-1)
        {
            PreparaFragmento(Estado, (BYTE*)Temporal, 4*(j+1));
            j=-1;
        }
    }
    PreparaUltimoFragmento(Estado, Temporal);
    Copia(Temporal, R, TamRW);
    R[4*TamRW - 4] >>= Despl;
}


/*****************************************************************************
FUNCION: GetCharBit
DESCRIPCION: Accede al bit NumBit-ésimo de A.
*****************************************************************************/
int GetCharBit(unsigned char *A, int NumBit)
{
    unsigned char Res = A[NumBit >> 3];
    Res >>= (NumBit % 8);
    return (Res & 1);
}


/*****************************************************************************
FUNCION: CheckLic
DESCRIPCION:
*****************************************************************************/
int CheckLic(DWORD *NumMagicos, BYTE *ClaveInf, int TamClave, BYTE *ClaveSup)
{
    DWORD *Mod, *UnoCero, *A1A2, *B1B2;
    DWORD *R1, *R2, *R3, *R4;
    DWORD CSup1[4], CSup2[8];       // Realmente son 1 y 2, pero mejor que sobre
    int Tam, TamCSup1, TamCSup2;
    int Resultado;

    // Lee la tabla de números mágicos
    if(LeeTablaMagica(NumMagicos, &Mod, &UnoCero, &A1A2, &B1B2, &Tam, &TamCSup1,
        &TamCSup2) == ERROR_NUMMAGICOS)
        return ERROR_NUMMAGICOS;

    // Certifica números mágicos
    if(!CompruebaEnCurvaEliptica(A1A2, UnoCero, Mod, Tam))
        return ERROR_NUMMAGICOS;
    if(!CompruebaEnCurvaEliptica(B1B2, UnoCero, Mod, Tam))
        return ERROR_NUMMAGICOS;

    // Limpia destinos CSup1 y CSup2
    for(int i=0; i<4; i++)
        CSup1[i] = 0;
    for(int i=0; i<8; i++)
        CSup2[i] = 0;
    // Copia de bytes a dwords
    for(int i=0; i<TamCSup1; i++)
        SetBit(CSup1, i, GetCharBit(ClaveSup, i));
    for(int i=TamCSup1; i<TamCSup1+TamCSup2; i++)
        SetBit(CSup2, i-TamCSup1, GetCharBit(ClaveSup, i));

    R1 = new DWORD[Tam*2];
    R2 = new DWORD[Tam*2];
    R3 = new DWORD[Tam*2];
    R4 = new DWORD[Tam*2];

    MultiplicaEnCurvaEliptica(A1A2, CSup2, TamCSup2, R1, UnoCero, Mod, Tam);
    MultiplicaEnCurvaEliptica(B1B2, CSup1, TamCSup1, R2, UnoCero, Mod, Tam);
    SumaEnCurvaEliptica(R1, R2, R3, UnoCero, Mod, Tam);
    ProcesaSHA1((DWORD*)ClaveInf, TamClave, R3, Tam*2, R4, TamCSup1);

    Resultado = (Compara(R4, CSup1, 1) == 0)? OK : MAL;

    delete [] R4;
    delete [] R3;
    delete [] R2;
    delete [] R1;
    return Resultado;
}


/*****************************************************************************
FUNCION: SetCharBit
DESCRIPCION: Fija el bit NumBit-ésimo de A.
*****************************************************************************/
void SetCharBit(unsigned char *A, int NumBit, int Valor)
{
    unsigned char Mask = 1 << (NumBit % 8);
    if(Valor)
        A[NumBit >> 3] |= Mask;
    else
        A[NumBit >> 3] &= ~Mask;
}


/*****************************************************************************
FUNCION: GenLic
DESCRIPCION:
*****************************************************************************/
int GenLic(DWORD *NumMagicos, DWORD *G1G2, DWORD *s1, DWORD *s2, DWORD *f,
            BYTE *ClaveInf, int TamClave, BYTE *ClaveSup)
{
    DWORD *Mod, *UnoCero, *A, *B;
    DWORD A1A2[2*12], B1B2[2*12];
    DWORD *R1, *R3, *R4;
    DWORD *CSup1, *CSup2;
    static BYTE TablaRandom[258];
    int Tam, TamCSup1, TamCSup2;
    bool Repetir;
    // Tamaño de f en bytes. Tiene que dar cabida al valor "a" o "CSup2"
    // En este caso, es fijo a 55 bits, o sea, 7 bytes
    const int TAM_F = 7;
    static bool inicializado = false;

    // Lee la tabla de números mágicos
    if(LeeTablaMagica(NumMagicos, &Mod, &UnoCero, &A, &B, &Tam, &TamCSup1,
        &TamCSup2) == ERROR_NUMMAGICOS)
        return ERROR_NUMMAGICOS;

    // Certifica números mágicos
    if(!CompruebaEnCurvaEliptica(G1G2, UnoCero, Mod, Tam))
        return ERROR_NUMMAGICOS;

    R1 = new DWORD[Tam*2];
    CSup1 = new DWORD[Tam];
    CSup2 = new DWORD[Tam];
    R3 = new DWORD[Tam*2];
    R4 = new DWORD[Tam*2];

    // Genera A1A2, B1B2 para asegurarse de que son correctos
    // de forma que A = s1*G, y B = s2*G
    MultiplicaEnCurvaEliptica(G1G2, s1, 32, A1A2, UnoCero, Mod, Tam);
    MultiplicaEnCurvaEliptica(G1G2, s2, 32, B1B2, UnoCero, Mod, Tam);
    // Vuelve a generar las tablas por si te has pasao de listo con A y B
    if(Compara(A1A2, A, 24) || Compara(B1B2, B, 24))
        return ERROR_NUMMAGICOS;

    // Inicializa los números aleatorios (supondremos F 56 bits)
    if(!inicializado)
    {
        Generador(TablaRandom, (BYTE*)f, TAM_F);
        inicializado = true;
    }

    do
    {
        // Escoge F al azar, de 55 bits. Dado que genera 56, borra el mayor
        Modificador(TablaRandom, (BYTE*)f, TAM_F);
        SetBit(f, 55, 0);

        // Genera valor suma de la curva elíptica  X = f*G = A*csup2 + B*csup1
        // entonces X = s1*G*csup2 + s2*G*csup1 = (s1*csup2 + s2*csup1)G = fG
        MultiplicaEnCurvaEliptica(G1G2, f, TAM_F << 3, R3, UnoCero, Mod, Tam);
        // Aplica SHA para saber cuál es el valor inferior
        ProcesaSHA1((DWORD*)ClaveInf, TamClave, R3, Tam*2, R4, TamCSup1);
        // Copia el resultado obtenido a CSup1 (sólo el tamaño que nos interese)
        for(int i=0; i<Tam; i++)
            CSup1[i] = 0;
        Copia(R4, CSup1, (TamCSup1 >> 5) + 1);
        // Podemos despejar csup2 de f = s1*csup2 + s2*csup1
        // Calculamos s2*csup1.
        MultiplicaModulo(s2, CSup1, R1, Mod, Tam);
        // Resta s1*csup2 = f - s2*csup1
        RestaModulo(f, R1, R1, Mod, Tam);
        // Divide... csup2 = (f - s2*csup1) / s1
        DivideModulo(R1, s1, CSup2, Mod, Tam);

        // Comprueba que el resultado no se sale del tamaño de CSup2
        Repetir = false;
        for(int i=TamCSup2; i<(Tam << 5); i++)
            if(GetBit(CSup2, i))
                Repetir = true;

    } while(Repetir);

    // Limpia lugar del resultado
    for(int i=0; i< ((TamCSup1+TamCSup2) >> 3)+1; i++)		// PARENTESIS ANADIDO
        ClaveSup[i] = 0;
    // Convierte de dwords a bytes CSup2 y CSup1
    for(int i=0; i<TamCSup1; i++)
        SetCharBit(ClaveSup, i, GetBit(CSup1, i));
    for(int i=TamCSup1; i<TamCSup1+TamCSup2; i++)
        SetCharBit(ClaveSup, i, GetBit(CSup2, i-TamCSup1));

    delete [] R4;
    delete [] R3;
    delete [] CSup1;
    delete [] CSup2;
    delete [] R1;

    return CheckLic(NumMagicos, ClaveInf, TamClave, ClaveSup);
}

