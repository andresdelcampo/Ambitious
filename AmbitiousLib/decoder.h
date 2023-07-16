/*****************************************************************************
MODULO: decoder
DESCRIPCION: Funciones de codificación/decodificación entre clave y magnitud
*****************************************************************************/

#ifndef __DECODER_H
#define __DECODER_H


#define DWORD   unsigned int
#define BYTE    unsigned char

int ClaveMagnitud(BYTE *Clave, BYTE *MagSup, int TamSup, BYTE *MagInf);
int MagnitudClave(BYTE *Clave, BYTE *MagSup, int TamSup, BYTE *MagInf);
#endif
 