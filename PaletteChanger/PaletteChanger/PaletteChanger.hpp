#pragma once
#include <vector>
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

class Color
{
public:
	Color() {};
	Color(float red, float green, float blue)
	{
		r = red;
		g = green;
		b = blue;
		rgb2hsv();
	}
	~Color() {};
	float r, g, b, h, s, v;
	void rgb2hsv();
	void hsv2rgb();
private:


};

Color TempColor; //this is dirty but im short on time

bool hueSort(Color &i, Color &j) {
	return (abs(i.h - TempColor.h) < abs(j.h - TempColor.h));
}

bool saturationSort(Color &i, Color &j) {
	return (abs(i.s - TempColor.s) <abs(j.s - TempColor.s));
}
bool brightnessSort(Color &i, Color &j) {
	return (abs(i.v - TempColor.v) < abs(j.v - TempColor.v));
}
namespace std
{
	template<> struct less<Color>
	{
		bool operator() (const Color& lhs, const Color& rhs) const
		{
			return lhs.r < rhs.r;
		}
	};
}

class PaletteChanger
{
public:
	PaletteChanger() {};
	~PaletteChanger() {};
	void AddPalette(float* rgbFloats,int size)
	{
		float* point = rgbFloats;
		for (int i = 0; i < size; i++)
		{
			Color c;
			c.r = *point;
			point++;
			c.g = *point;
			point++;
			c.b = *point;
			c.rgb2hsv();
			palette.emplace(palette.begin(),c);
		}

	}
	Color SwapPixels(Color in)
	{
		if (colorMap.count(in) > 0)
		{
			colorMap.at(in);
		}
		TempColor = in;
		sort(palette.begin(),palette.end(), brightnessSort);
		int half = palette.size() / 2;
		sort(palette.begin(), palette.begin() + half, hueSort);
		half /= 2;
		sort(palette.begin(), palette.begin() + half, saturationSort);

		Color out = palette.at(0);
		colorMap.emplace( in, out );

		return out;
	}
private:
	vector<Color> palette;
	map<Color, Color> colorMap;
};



//stole this from stackoverflow
void Color::rgb2hsv()
{
	double      min, max, delta;

	min = this->r < this->g ? this->r : this->g;
	min = min < this->b ? min : this->b;

	max = this->r > this->g ? this->r : this->g;
	max = max > this->b ? max : this->b;

	this->v = max;                                // v
	delta = max - min;
	if (delta < 0.00001)
	{
		this->s = 0;
		this->h = 0; // undefined, maybe nan?
		return;
	}
	if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
		this->s = (delta / max);                  // s
	}
	else {
		// if max is 0, then r = g = b = 0
		// s = 0, h is undefined
		this->s = 0.0;
		this->h = NAN;                            // its now undefined
		return;
	}
	if (this->r >= max)                           // > is bogus, just keeps compilor happy
		this->h = (this->g - this->b) / delta;        // between yellow & magenta
	else
		if (this->g >= max)
			this->h = 2.0 + (this->b - this->r) / delta;  // between cyan & yellow
		else
			this->h = 4.0 + (this->r - this->g) / delta;  // between magenta & cyan

	this->h *= 60.0;                              // degrees

	if (this->h < 0.0)
		this->h += 360.0;

	return;
}

void Color::hsv2rgb()
{
	double      hh, p, q, t, ff;
	long        i;

	if (this->s <= 0.0) {       // < is bogus, just shuts up warnings
		this->r = this->v;
		this->g = this->v;
		this->b = this->v;
		return;
	}
	hh = this->h;
	if (hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = this->v * (1.0 - this->s);
	q = this->v * (1.0 - (this->s * ff));
	t = this->v * (1.0 - (this->s * (1.0 - ff)));

	switch (i) {
	case 0:
		this->r = this->v;
		this->g = t;
		this->b = p;
		break;
	case 1:
		this->r = q;
		this->g = this->v;
		this->b = p;
		break;
	case 2:
		this->r = p;
		this->g = this->v;
		this->b = t;
		break;

	case 3:
		this->r = p;
		this->g = q;
		this->b = this->v;
		break;
	case 4:
		this->r = t;
		this->g = p;
		this->b = this->v;
		break;
	case 5:
	default:
		this->r = this->v;
		this->g = p;
		this->b = q;
		break;
	}
	return;
}
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
