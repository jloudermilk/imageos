
#include "PaletteChanger.hpp"

PaletteChanger* changer;
 
	extern "C" bool Startup()
	{
		changer = new PaletteChanger();
		return false;
	}
	extern "C" void ChangePalette(float* rgb, int size)
	{
		changer->AddPalette(rgb,size);

	}
	extern "C" unsigned char* SwapPalettes(unsigned char* rgbBytes,int length)
	{
		Color c = Color(1.0f, 0.0f, 0.0f);
		unsigned char* bytes = rgbBytes;

		for (int i = 0; i < length; i+=3)
		{
			c = Color(bytes[i], bytes[i + 1], bytes[i + 2]);
			c = changer->SwapPixels(c);
			bytes[i] = c.r * 255;
			bytes[i+1] = c.g * 255;
			bytes[i+2] = c.b * 255;
		}
		return bytes;
	}
	extern "C" unsigned char* SubmitImage(unsigned char* rgbBytes)
	{
		return rgbBytes;
	}
