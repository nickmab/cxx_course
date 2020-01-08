#pragma once

namespace mabz { namespace calculators {

// abstract base class for going from (x, y) cartesian coordinates
// to a pixel "score" according to a function (e.g. mandelbrot iterations)
// which can then be used to colorize the pixel in a bmp (for example).
class PixelScoreCalculator
{
public:
	virtual void GetPixelScore(int& outResult, double x, double y) const = 0;
};

} /* namespace calculators */
} /* namespace mabz */
