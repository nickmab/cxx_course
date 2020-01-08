#pragma once

#include <advanced_cxx/colors/color_utils.h>

namespace mabz { namespace color {

#define ui8 std::uint8_t

// abstract base class for going from a "score" for a pixel (e.g. something
// representing what intensity/style of color it should have) to an actual RGB.
class ColorScheme
{
public:
	virtual RGB GetColor(int score) const = 0;
	virtual ~ColorScheme() {}
};

// This was developed for colorizing the mandelbrot set, where you have
// one color for a "maxed out" score (i.e. if it's in the set) and 
// a color scale if it's not in the set, which depends on how quickly it diverges.
class SingleColorScheme : public ColorScheme
{
private:
	int mScalingDenominator{0};
	RGB mHundredPerCentColor;
	HSV mScalingBaseColor;

public:
	// mScalingDenominator, RGB of mHundredPerCentColor, RGB of mScalingBaseColor.
	SingleColorScheme(int, ui8, ui8, ui8, ui8, ui8, ui8);
	
	virtual RGB GetColor(int score) const override;
};

class DodgyColorScheme : public ColorScheme
{
private:
	RGB mSolidColor;

public:
	DodgyColorScheme(ui8 r, ui8 g, ui8 b) : mSolidColor(r, g, b) {}

	// just to check the vtable works ok...
	virtual RGB GetColor(int) const override
	{
		return mSolidColor;
	}
};

#undef ui8

} /* namespace color */
} /* namespace mabz */
