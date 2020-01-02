#ifndef ADVANCED_CXX_IMG_SCALER_H
#define ADVANCED_CXX_IMG_SCALER_H

#include <cassert>

namespace mabz {

// Used to map from one scale to another, e.g. if you have an x,y range of 
// a cartesian plane / graph kind of thing and you want to turn it into an 800 x 800 image...
// Could later be turned into a generic to map from type A to type B.
class ImgScaler
{
private:
	const double mSourceMin{0};
	const double mSourceRange{0};
	const int mDestMin{0};
	const int mDestRange{0};

public:
	ImgScaler() = delete;
	ImgScaler(double sourceMin, double sourceMax, int destMin, int destMax)
		: mSourceMin(sourceMin)
		, mSourceRange(sourceMax - sourceMin)
		, mDestMin(destMin)
		, mDestRange(destMax - destMin)
	{
		assert (mSourceRange > 0);
		assert (mDestRange > 0);
	}
	ImgScaler(const ImgScaler&) = default;
	~ImgScaler() = default;

	// Simply a linear extrapolation based on distance from the midpoint of 
	// the range; does not bother to check whether the provided 'in' value is within source range!!
	void Convert(double in, int& out) const;
};

} /* namespace mabz */

#endif /* ADVANCED_CXX_IMG_SCALER_H */