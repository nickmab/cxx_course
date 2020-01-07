#pragma once

#include <cassert>

namespace mabz {

// Used to map from one scale to another, e.g. if you have x,y pixel coordinates in
// an 800 x 600 image and you want to map them to an (x,y) cartesian plane from [-1, +1]
// Could later be turned into a generic to map from type A to type B.
class ImgScaler
{
private:
	const int mSourceMin{0};
	const int mSourceRange{0};
	const double mDestMin{0};
	const double mDestRange{0};

public:
	ImgScaler() = delete;
	ImgScaler(int sourceMin, int sourceMax, double destMin, double destMax)
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
	void Convert(int in, double& out) const;
};

} /* namespace mabz */
