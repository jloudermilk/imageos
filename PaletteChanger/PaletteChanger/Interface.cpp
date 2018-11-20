
#include "PaletteChanger.hpp"

PaletteChanger* changer;

extern bool Startup() 
{
	changer = new PaletteChanger();
	return false;
}
extern void AddPalette(float* hsv,int size) 
{
	changer->AddPalette(hsv,size);

}
extern unsigned char* SwapPalettes(unsigned char* rgbBytes) 
{
	Color c = Color(0, 0, 0);
	changer->SwapPixels(c);
	return rgbBytes;
}