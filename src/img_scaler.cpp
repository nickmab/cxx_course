#include "advanced_cxx/img_scaler.h"

namespace mabz { 

void ImgScaler::Convert(double in, int& out) const
{
	const double inLeftToRightProportion = (in - mSourceMin) / mSourceRange;
	// this is a crude "rounding" technique to go from double to the nearest integer.
	out = mDestMin + static_cast<int>(0.5 + inLeftToRightProportion * static_cast<double>(mDestRange));
}

} /* namespace mabz */