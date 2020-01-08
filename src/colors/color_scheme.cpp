#include <utility>

#include "advanced_cxx/colors/color_scheme.h"

namespace mabz { namespace color {

#define ui8 std::uint8_t
SingleColorScheme::SingleColorScheme(
	int scalingDenominator, 
	ui8 rFull, ui8 gFull, ui8 bFull, 
	ui8 rBase, ui8 gBase, ui8 bBase)
	
	: mScalingDenominator(scalingDenominator)
	, mHundredPerCentColor(rFull, gFull, bFull)
	, mScalingBaseColor(std::move(RGB::ToHSV(rBase, gBase, bBase)))
{}
#undef ui8

RGB SingleColorScheme::GetColor(int score) const
{
	if (score == mScalingDenominator)
	{
		// makes / returns a copy obviously.
		return mHundredPerCentColor;
	}
	else
	{
		const double intensity = sqrt(static_cast<double>(score) / mScalingDenominator);
		HSV hsv(mScalingBaseColor);
		hsv.mHue *= intensity;
		hsv.mSaturation *= intensity;
		hsv.mValue *= intensity;
		return std::move(hsv.ToRGB());
	}
}

} /* namespace color */
} /* namespace mabz */