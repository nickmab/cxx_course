#ifndef ADVANCED_CXX_COLOR_UTILS_H
#define ADVANCED_CXX_COLOR_UTILS_H

#include <cstdint>

namespace mabz { namespace color {

#define ui8 std::uint8_t

// forward dec -> these structs refer to one another.
struct HSV;
struct RGB;

struct RGB
{
	ui8 mRed{0};
	ui8 mGreen{0};
	ui8 mBlue{0};

	RGB(ui8 r, ui8 g, ui8 b) : mRed(r), mGreen(g), mBlue(b) {}
	RGB(const RGB&) = default;
	RGB(RGB&& rgb) : mRed(rgb.mRed), mGreen(rgb.mGreen), mBlue(rgb.mBlue) {}

	static HSV ToHSV(ui8 r, ui8 g, ui8 b);
	HSV ToHSV() const;
};

struct HSV
{
	double mHue{0}; // values from [0,360) degrees.
	double mSaturation{0}; // values from 0.0 to 1.0 (representing per cent).
	double mValue{0}; // values from 0.0 to 1.0 (representing per cent).

	HSV(double h, double s, double v) : mHue(h), mSaturation(s), mValue(v) {}
	HSV(const HSV&) = default;
	HSV(HSV&& hsv) : mHue(hsv.mHue), mSaturation(hsv.mSaturation), mValue(hsv.mValue) {}

	RGB ToRGB() const;
	static RGB ToRGB(double h, double s, double v);
};

#undef ui8

} /* namespace color */
} /* namespace mabz */

#endif /* ADVANCED_CXX_COLOR_UTILS_H */