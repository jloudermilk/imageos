
#include "PaletteChanger.hpp"

PaletteChanger* changer;
 
	extern "C" bool Startup()
	{
		changer = new PaletteChanger();
		return false;
	}
	extern "C" void AddPalette(float* hsv, int size)
	{
		//changer->AddPalette(hsv,size);

	}
	extern "C" unsigned char* SwapPalettes(unsigned char* rgbBytes)
	{
		Color c = Color(0, 0, 0);
		changer->SwapPixels(c);
		return rgbBytes;
	}
	extern "C" unsigned char* SubmitImage(unsigned char* rgbBytes)
	{
		return rgbBytes;
	}
