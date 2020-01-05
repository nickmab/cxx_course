#include "advanced_cxx/color_map.h"

#include <algorithm>
#include <cassert>

namespace mabz { namespace color {

#define ui8 std::uint8_t

void HistogramBased::AddColorZone(ui8 r, ui8 g, ui8 b) 
{ 
	mColorZones.emplace_back(r, g, b); 
	mNumColorZones++; // put this on its own line just for clarity.
	mZoneCutoffStepSize = 1.0 / static_cast<double>(mNumColorZones);
	mColorZoneCutoffs.clear();
	for (int i = 0; i < mNumColorZones; ++i)
	{
		mColorZoneCutoffs.push_back((i+1) * mZoneCutoffStepSize);
	}
}

#undef ui8

RGB HistogramBased::GetColor(double percentageBelowThisColor) const
{
	int i = 0;
	for ( ; i < mNumColorZones; i++)
	{
		if (mColorZoneCutoffs[i] > percentageBelowThisColor) break;
	}
	const RGB& baseColor = mColorZones[i];
	const double scaleFactor = 
			(i == 0 
			? percentageBelowThisColor 
			: percentageBelowThisColor - mColorZoneCutoffs[i-1])
			/ mZoneCutoffStepSize;

	return std::move(baseColor * std::min(0.25, scaleFactor));
}

} /* namespace color */
} /* namespace mabz */