#ifndef ADVANCED_CXX_FRACTAL_H
#define ADVANCED_CXX_FRACTAL_H

#include <memory>

#include <advanced_cxx/bmp.h>
#include <advanced_cxx/img_scaler.h>

namespace mabz { namespace mandelbrot {

class FractalBmp
{
private:
	mabz::Bmp mBmp;
	// use unique_ptr to get the initialization sequence right - 
	// it has no default constructor and we need to calculate some properties on the fly.
	std::unique_ptr<mabz::ImgScaler> mXScaler;
	std::unique_ptr<mabz::ImgScaler> mYScaler;
	double mXMin{0};
	double mXMax{0};
	double mYMin{0};
	double mYMax{0};

public:
	FractalBmp() = delete;
	FractalBmp(
		double xCenter, 
		double yCenter, 
		double xDomainWidth, 
		int pixelWidth,
		int pixelHeight
	);
	FractalBmp(const FractalBmp&) = delete;

	// true or false indicates success or failure...
	// although at the moment I am not doing proper error checking, basically just returning true lol.
	bool Generate();
	bool WriteToFile(const char* filename) const;
};

} /* namespace mandelbrot */
} /* namespace mabz */

#endif /* ADVANCED_CXX_FRACTAL_H */