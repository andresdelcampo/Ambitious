// AmbitiousLib.h

#pragma once

using namespace System;

namespace AmbitiousLib {

	public ref class KeyServices
	{
		public:

		 static int ValidateKey(unsigned char key[], int productIndex, 
								unsigned int* Mod, unsigned int* A1A2, unsigned int* B1B2,
								unsigned int* UpperKey1, unsigned int* UpperKey2, unsigned int* UpperKey3, unsigned int* UpperKey4, 
								unsigned int* LowerKey);
		 static int KeyServices::SearchProductFromKey(unsigned char key[], 
							 unsigned int* UpperKey1, unsigned int* UpperKey2, unsigned int* UpperKey3, unsigned int* UpperKey4, 
							 unsigned int* LowerKey);
		 static unsigned int* Index2Product(int productIndex);
		 static int GeneratePID(unsigned int* PublicKey, unsigned int* GxGy, unsigned int* s1, unsigned int* s2);
		 static int GenerateKey(unsigned char** key,
							 unsigned int* Mod, unsigned int* A1A2, unsigned int* B1B2,
							 unsigned int* LowerKey,
							 unsigned int* GxGy, unsigned int* s1, unsigned int* s2, unsigned int* fLow, unsigned int* fHigh,
							 unsigned int* UpperKey1, unsigned int* UpperKey2, unsigned int* UpperKey3, unsigned int* UpperKey4);
	};
}
