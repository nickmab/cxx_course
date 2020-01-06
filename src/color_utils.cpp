#include "advanced_cxx/color_utils.h"

#include <algorithm>
#include <cmath>

namespace mabz { namespace color {

#define ui8 std::uint8_t

HSV RGB::ToHSV(ui8 r, ui8 g, ui8 b)
{
	const double rPrime = static_cast<double>(r)/255.0;
	const double gPrime = static_cast<double>(g)/255.0;
	const double bPrime = static_cast<double>(b)/255.0;
	
	double h{0};
	double s{0};
	double v{0};

	if (r == g && g == b)
	{
		// hue and saturation remain at 0.
		v = rPrime; // max(rPrime, gPrime, bPrime) -> but they're all the same.
	}
	else
	{
		if (g > r)
		{
			if (b > g)
			{
				// blue is max and red is min.
				v = bPrime;
				const double delta = bPrime - rPrime;
				h = 60 * ((rPrime - gPrime) / delta + 4);
				s = delta / bPrime;
			}
			else
			{
				// green is max
				v = gPrime;
				if (b < r)
				{
					// blue is min
					const double delta = gPrime - bPrime;
					h = 60 * ((bPrime - rPrime) / delta + 2);
					s = delta / gPrime;
				}
				else
				{
					// red is min
					const double delta = gPrime - rPrime;
					h = 60 * ((bPrime - rPrime) / delta + 2);
					s = delta / gPrime;
				}
			}
		}
		else
		{
			if (r > b)
			{
				// red is max
				v = rPrime;
				if (b > g)
				{
					// green is min
					const double delta = rPrime - gPrime;
					h = 60 * std::fmod((gPrime - bPrime) / delta, 6);
					s = delta / rPrime;
				}
				else
				{
					// blue is min
					const double delta = rPrime - bPrime;
					h = 60 * std::fmod((gPrime - bPrime) / delta, 6);
					s = delta / rPrime;
				}
			}
			else
			{
				// blue is max and green is min
				v = bPrime;
				const double delta = bPrime - gPrime;
				h = 60 * ((rPrime - gPrime) / delta + 4);
				s = delta / bPrime;
			}
		}
	}

	return std::move(HSV(h, s, v));
}

HSV RGB::ToHSV() const
{
	return std::move(RGB::ToHSV(mRed, mGreen, mBlue));
}

RGB HSV::ToRGB(double h, double s, double v)
{
	const double C = v * s;
	const double X = C * (1 - abs(std::fmod(h / 60, 2) - 1));
	const double m = v - C;

	double rPrime{0};
	double gPrime{0};
	double bPrime{0};

	if (h < 60)
	{
		rPrime = C;
		gPrime = X;
		bPrime = 0;
	}
	else if (h < 120)
	{
		rPrime = X;
		gPrime = C;
		bPrime = 0;
	}
	else if (h < 180)
	{
		rPrime = 0;
		gPrime = C;
		bPrime = X;	
	}
	else if (h < 240)
	{
		rPrime = 0;
		gPrime = X;
		bPrime = C;
	}
	else if (h < 300)
	{
		rPrime = X;
		gPrime = 0;
		bPrime = C;
	}
	else
	{
		rPrime = C;
		gPrime = 0;
		bPrime = X;
	}

	return std::move(
		RGB(
			static_cast<ui8>(255 * (rPrime + m)),
			static_cast<ui8>(255 * (gPrime + m)),
			static_cast<ui8>(255 * (bPrime + m))
		));
}

RGB HSV::ToRGB() const
{
	return std::move(HSV::ToRGB(mHue, mSaturation, mValue));
}

#undef ui8

} /* namespace color */
} /* namespace mabz */