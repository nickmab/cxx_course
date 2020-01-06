#ifndef ADVANCED_CXX_FRACTAL_H
#define ADVANCED_CXX_FRACTAL_H

#include <map>
#include <memory>
#include <vector>

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

	void Generate();
	bool WriteToFile(const char* filename) const;
};

} /* namespace mandelbrot */
} /* namespace mabz */

#endif /* ADVANCED_CXX_FRACTAL_H */