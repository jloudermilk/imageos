#pragma once
#include <vector>
#include <math.h>
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>

typedef struct {
	double r;       // a fraction between 0 and 1
	double g;       // a fraction between 0 and 1
	double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
	double h;       // angle in degrees
	double s;       // a fraction between 0 and 1
	double v;       // a fraction between 0 and 1
} hsv;
hsv TempHSV; //this is dirty but im short on time

bool hueSort(hsv &i, hsv &j) {
	return (std::abs(i.h - TempHSV.h) < std::abs(j.h - TempHSV.h));
}

bool saturationSort(hsv &i, hsv &j) {
	return (std::abs(i.s - TempHSV.s) < std::abs(j.s - TempHSV.s));
}
bool brightnessSort(hsv &i, hsv &j) {
	return (std::abs(i.v - TempHSV.v) < std::abs(j.v - TempHSV.v));
}


static hsv   rgb2hsv(rgb in);
static rgb   hsv2rgb(hsv in);

class PaletteChanger
{
public:
	PaletteChanger() {};
	~PaletteChanger() {};
	std::vector<hsv> palette;
	std::map<rgb, hsv> colorMap;


private:
	rgb SwapPixels(const rgb in)
	{
		if (colorMap.count(in) > 0)
		{
			return hsv2rgb(colorMap.at(in));
		}
		TempHSV = rgb2hsv(in);
		std::sort(palette.begin(),palette.end(), brightnessSort);
		int half = palette.size() / 2;
		std::sort(palette.begin(), palette.begin() + half, hueSort);
		half /= 2;
		std::sort(palette.begin(), palette.begin() + half, saturationSort);
		colorMap.insert(std::pair<rgb,hsv>(in, palette.at(0)));
		rgb out = hsv2rgb(palette.at(0));
		return out;
	}
};


PaletteChanger changer;

extern unsigned char* SwapPalettes(unsigned char* rgbaBytes);

hsv rgb2hsv(rgb in)
{
	hsv         out;
	double      min, max, delta;

	min = in.r < in.g ? in.r : in.g;
	min = min < in.b ? min : in.b;

	max = in.r > in.g ? in.r : in.g;
	max = max > in.b ? max : in.b;

	out.v = max;                                // v
	delta = max - min;
	if (delta < 0.00001)
	{
		out.s = 0;
		out.h = 0; // undefined, maybe nan?
		return out;
	}
	if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
		out.s = (delta / max);                  // s
	}
	else {
		// if max is 0, then r = g = b = 0
		// s = 0, h is undefined
		out.s = 0.0;
		out.h = NAN;                            // its now undefined
		return out;
	}
	if (in.r >= max)                           // > is bogus, just keeps compilor happy
		out.h = (in.g - in.b) / delta;        // between yellow & magenta
	else
		if (in.g >= max)
			out.h = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
		else
			out.h = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

	out.h *= 60.0;                              // degrees

	if (out.h < 0.0)
		out.h += 360.0;

	return out;
}

rgb hsv2rgb(hsv in)
{
	double      hh, p, q, t, ff;
	long        i;
	rgb         out;

	if (in.s <= 0.0) {       // < is bogus, just shuts up warnings
		out.r = in.v;
		out.g = in.v;
		out.b = in.v;
		return out;
	}
	hh = in.h;
	if (hh >= 360.0) hh = 0.0;
	hh /= 60.0;
	i = (long)hh;
	ff = hh - i;
	p = in.v * (1.0 - in.s);
	q = in.v * (1.0 - (in.s * ff));
	t = in.v * (1.0 - (in.s * (1.0 - ff)));

	switch (i) {
	case 0:
		out.r = in.v;
		out.g = t;
		out.b = p;
		break;
	case 1:
		out.r = q;
		out.g = in.v;
		out.b = p;
		break;
	case 2:
		out.r = p;
		out.g = in.v;
		out.b = t;
		break;

	case 3:
		out.r = p;
		out.g = q;
		out.b = in.v;
		break;
	case 4:
		out.r = t;
		out.g = p;
		out.b = in.v;
		break;
	case 5:
	default:
		out.r = in.v;
		out.g = p;
		out.b = q;
		break;
	}
	return out;
}