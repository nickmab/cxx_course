#include "advanced_cxx/img_scaler.h"

namespace mabz { 

void ImgScaler::Convert(int in, double& out) const
{
	const double inLeftToRightProportion = (in - mSourceMin) / (double)mSourceRange;
	out = mDestMin + inLeftToRightProportion * mDestRange;
}

} /* namespace mabz */