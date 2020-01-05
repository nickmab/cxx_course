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

	// for use with colorization.
	// for each pixel, how many iterations did it need? Organised by row,
	// starting from the bottom left. i.e. pixel (x, y) is at (y*row_width + x)
	std::vector<int> mIterationPixelMap;
	// counts of how many pixels in the bmp required this number of iterations
	// e.g. if 100 pixels needed only 2 iterations, then you'd have {2: 100}
	std::map<int,int> mIterationsToPixelCount;
	// what percentage of pixels needed less than this number of iterations?
	// e.g. if 80% need less than 500, you would have {500: 0.8}
	std::map<int,double> mIterationsCumulativeDensity;

	void IterationsFirstPass();
	void IterationsSecondPass();

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
	void Generate();
	void Colorize();
	bool WriteToFile(const char* filename) const;
};

} /* namespace mandelbrot */
} /* namespace mabz */

#endif /* ADVANCED_CXX_FRACTAL_H */