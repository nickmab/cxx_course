#ifndef ADVANCED_CXX_COLOR_MAP_H
#define ADVANCED_CXX_COLOR_MAP_H

#include <cstdint>
#include <iostream>
#include <utility>
#include <vector>

namespace mabz { namespace color {

#define ui8 std::uint8_t

struct RGB
{
	ui8 mRed{0};
	ui8 mGreen{0};
	ui8 mBlue{0};

	RGB(ui8 r, ui8 g, ui8 b) : mRed(r), mGreen(g), mBlue(b) {}
	RGB(const RGB&) = default;
	RGB(RGB&& rgb) : mRed(rgb.mRed), mGreen(rgb.mGreen), mBlue(rgb.mBlue) {}
};

RGB operator * (double x, const RGB& rgb)
{
	return std::move(RGB(x*rgb.mRed, x*rgb.mGreen, x*rgb.mBlue));
}

RGB operator * (const RGB& rgb, double x)
{
	return std::move(RGB(x*rgb.mRed, x*rgb.mGreen, x*rgb.mBlue));
}

class HistogramBased
{
private:
	std::vector<RGB> mColorZones;
	std::vector<double> mColorZoneCutoffs;
	double mZoneCutoffStepSize;
	int mNumColorZones{0};

public:
	RGB mMandelbrotSetColor;
	
	HistogramBased() = delete;
	HistogramBased(ui8 mbsR, ui8 mbsG, ui8 mbsB)
		: mMandelbrotSetColor(mbsR, mbsG, mbsB)
	{}
	HistogramBased(const HistogramBased&) = delete;
	HistogramBased(HistogramBased&&) = delete;
	~HistogramBased() = default;

	void AddColorZone(ui8 r, ui8 g, ui8 b);
	RGB GetColor(double percentageBelowThisColor) const;
};

#undef ui8

} /* namespace color */
} /* namespace mabz */

#endif /* ADVANCED_CXX_COLOR_MAP_H */