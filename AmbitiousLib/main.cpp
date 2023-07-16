/*****************************************************************************
FUNCION: ambitious
DESCRIPCION:
*****************************************************************************/
#include "stdafx.h"
#include <stdio.h>
#include "main.h"
#include "decoder.h"
#include "checklic.h"
#include "magic.h"
//#include "protect.h"
#include "magicpid.h"
//---------------------------------------------------------------------------

// Ñieejejejejeje
static DWORD Proteccion[91] =
{0x0000016C,0x00000007,0x88A9BE20,0x0130DFAE,
0x0000000C,0x0000001C,0x00000037,
0x6D254F51,0x5841AFC1,0xC0AD6EF9,0xB4717C56,
0x352767DD,0xA4BC3208,0x015EFB5C,0x6B69C8F8,
0x9702503E,0x5966DB1A,0x5ACF83BA,0x85FC84B7,
0x00000001,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,
0x00000000,0x00000000,0x00000000,0x00000000,
0x85AB2686,0xFF785F6C,0x7F9601AA,0xE829EA6C,
0xC82CDD41,0x1F3C87DD,0xD0822810,0x593C4DED,
0x259EE808,0x3A42525A,0xCA919999,0x611FF419,
0xA9BCB852,0x984951B9,0x868D8813,0x28AFF09B,
0xED780E2E,0x1676DEF6,0x6F7C1105,0xCCB65348,
0x222AAF2C,0x12606A09,0xDD6B3E5D,0x659C1FC9,
0x99A8601E,0x04F89FB8,0xC6B0D644,0xABFED4E4,
0xBDD80AD3,0xADF6329A,0x34F35245,0x9D8EAD18,
0xB253B9EC,0x9BEF5CC6,0xAD292C18,0x057D4946,
0x4A5B6FC1,0x1B99739B,0xE7009406,0x33EF95AF,
0xF1E57811,0x5A72E358,0xF0405E5F,0x7F2F3DF5,
0xB06FB1B3,0xADDE7A91,0x8F481A7C,0x765AC3DA};


/* Claves válidas
CJ6BY-JVH7X-QJQQG-B3TMD-7T26B	JR (7F00 0020)
C6H34-3GF22-XYQTY-KVXFJ-3GFWY   JA (7F01 0020)
B3272-YRMB7-T9JQ8-4YG44-FJHMY	JA (7F01 0010)
B6TTW-GDK6C-2HQXF-CDCKQ-KKP4K	Pública (0000 000F)

F2RDT-CW4PW-GTP7J-XK2TC-9P3YH   5 usos (act 0)
B6TTW-GDK6C-2HQXF-CDCKQ-KKP4K   15 usos (act 0)
BQWQ8-84W9H-FMWTX-C8B3Q-RJHMX   15 usos (act 0)
FHP6F-P7PM8-XHRP9-YJ8Q6-23YDY   64 usos (act 0)
CP4MT-P4Q7V-J99GM-CVXG3-VXFDX   255 usos (act 0)
DRHFB-HBRH8-K3FMF-FTPJ9-3RJX9   255 usos (act 0)
CF6M9-2HBVQ-2BQDP-W4C6Y-RJHMH   5 usos (act 1)
CCM9G-7YCJ9-TVTPH-RM72F-JQC37   5 usos (act 11)
FTDX2-RCCKD-KW79T-P2QJQ-RXY7Y   8 usos (act 12)
D824C-W7V2G-D3HVK-CBHKF-G6FVK   ilimitada (act 20)
*/

/* CODIGOS DE GENERACION

8

3523454

0xFDABEF33,0x24C18204,0x196836A2,0xE13E44D3,
0xC2C5891B,0x9A9EB51C,0x185DD3D3,0xD6FB7F83,
0x191F0AA2,0xD9743E8F,0x45790A6E,0x34305838;

0x61068A51,0x24C5DFE0,0x87254F21,0xCCD34393,
0x16EE20F9,0xECD03F64,0x59711570,0x565188D8,
0xFD7DA722,0xD27B81CF,0xA4813D52,0x570C8E39;

*/

//
///*****************************************************************************
//FUNCION: CrackerClick
//DESCRIPCION:
//*****************************************************************************/
//#include "aritmet.h"
//void __fastcall TFAmbitious::CrackerClick(TObject *Sender)
//{
//    DWORD G1G2[12*2];
//    DWORD s1 = 1, s2 = 1;
//    DWORD *Mod, *UnoCero, *A1A2, *B1B2;
//    int Tam, TamCSup1, TamCSup2;
//    int Resultado;
//
//    CargarMagicUsuario();
//
//    // Lee la tabla de números mágicos
//    if(LeeTablaMagica(Producto, &Mod, &UnoCero, &A1A2, &B1B2, &Tam, &TamCSup1,
//        &TamCSup2) == ERROR_NUMMAGICOS)
//        return;
//
//    DWORD test = 11001;
//    MultiplicaEnCurvaEliptica(A1A2, &test, 32, B1B2, UnoCero, Mod, Tam);
//
//try
//{
//    S2->Lines->LoadFromFile("C:\\ambitious.log");
//    s2 = S2->Text.ToInt();
//}
//catch(...)
//{
//    s2 = 1;
//}
//    MultiplicaEnCurvaEliptica(A1A2, &s2, 32, G1G2, UnoCero, Mod, Tam);
//
////    Copia(A1A2, G1G2, Tam*2);
//    do
//    {
//        SumaEnCurvaEliptica(G1G2, A1A2, G1G2, UnoCero, Mod, Tam);
//        s2++;
//        if(! (s2 % 100))
//        {
//            S2->Text = AnsiString(s2);
//            Update();
//            if(! (s2 % 10000))
//                S2->Lines->SaveToFile("C:\\ambitious.log");
//        }
//    }
//    while(Compara(G1G2, B1B2, Tam*2) != 0);
//
//    S1->Text = s1;
//    S2->Text = s2;
//    Update();
//    Beep();
//try
//{
//    S2->Lines->SaveToFile("C:\\ambitious.log");
//}catch(...){};
//}
//
//
//
//
