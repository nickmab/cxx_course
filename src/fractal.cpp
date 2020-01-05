#include <cstdint>

#include "advanced_cxx/fractal.h"

#include "advanced_cxx/color_map.h"
#include "advanced_cxx/mandelbrot.h"

namespace mabz { namespace mandelbrot {

FractalBmp::FractalBmp(
		double xCenter, 
		double yCenter, 
		double xDomainWidth, 
		int pixelWidth,
		int pixelHeight
	)
	: mBmp(pixelWidth, pixelHeight)
	, mIterationPixelMap(pixelWidth*pixelHeight, 0)
{
	const double aspectRatio = static_cast<double>(pixelHeight) / pixelWidth;
	const double xMidptOffset = 0.5 * xDomainWidth;
	const double yMidptOffset = aspectRatio * xMidptOffset;
	
	mXScaler.reset(new mabz::ImgScaler(
		0, pixelWidth - 1, xCenter - xMidptOffset, xCenter + xMidptOffset));
	mYScaler.reset(new mabz::ImgScaler(
		0, pixelHeight - 1, yCenter - yMidptOffset, yCenter + yMidptOffset));
}

// calls the Mandelbrot function and populates mIterationPixelMap
// and mIterationsToPixelCount.
void FractalBmp::IterationsFirstPass()
{
	const int width = mBmp.Width();
	const int height = mBmp.Height();
	
	std::cout << "Running IterationsFirstPass on " 
			  << width * height << " pixels." << std::endl;

	int pixelsInMandelbrotSet{0};
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			// determine real and imaginary parts corresponding to the pixel we're working with...
			double scaledX{0};
			double scaledY{0};
			mXScaler->Convert(x, scaledX);
			mYScaler->Convert(y, scaledY);

			// determine the mandelbrot iterations...
			int iterations = 0;
			if (calculator::IsDivergent(iterations, scaledX, scaledY))
			{
				// only store statistics about frequency if it diverges,
				// i.e. if it's not actually in the set (color for in-the-set is hard-coded).
				mIterationsToPixelCount[iterations]++;
			}
			else
			{
				pixelsInMandelbrotSet++;
			}
			mIterationPixelMap[y*width + x] = iterations;
		}
	}

	std::cout << "Found " << pixelsInMandelbrotSet << " pixels in the Mandelbrot set." << std::endl;
}

// Must only call after IterationsFirstPass. Uses mIterationPixelMap
// and mIterationsToPixelCount to populate mIterationsCumulativeDensity.
void FractalBmp::IterationsSecondPass()
{
	std::cout << "Running IterationsSecondPass." << std::endl;
	const double numPixels = static_cast<double>(mBmp.Width() * mBmp.Height());
	double pixelsCumSum{0};
	for (auto iterAndPixel : mIterationsToPixelCount)
	{
		mIterationsCumulativeDensity[iterAndPixel.first] = pixelsCumSum/numPixels;
		pixelsCumSum += iterAndPixel.second;
	}
	std::cout << "Sanity check on pixelsCumSum: " << pixelsCumSum << std::endl;
	std::cout << "Must mean there are " << numPixels - pixelsCumSum 
		      << " pixels in the Mandelbrot set." << std::endl;
}

// Must only run after the IterationsFirst/SecondPass functions. 
// Uses the data there to colorize according to how frequent the
// iteration counts are. 
void FractalBmp::Colorize()
{
	const int width = mBmp.Width();
	const int height = mBmp.Height();
	const int pixelMapSize = mIterationPixelMap.size();

	std::cout << "colorizing..." << std::endl;
	std::cout << "mIterationPixelMap has " << pixelMapSize
			  << " elements." << std::endl;

	mabz::color::HistogramBased histColor(40, 210, 95);
	histColor.AddColorZone(180, 10, 10);
	histColor.AddColorZone(90, 23, 185);

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			const int iterations = mIterationPixelMap[y*width + x];
			if (iterations == mabz::mandelbrot::calculator::MAX_ITERATIONS)
			{
				mBmp.SetRGBPixel(x, y, 
					histColor.mMandelbrotSetColor.mRed,
					histColor.mMandelbrotSetColor.mGreen,
					histColor.mMandelbrotSetColor.mBlue);
			}
			else
			{
				const mabz::color::RGB rgb = histColor.GetColor(mIterationsCumulativeDensity[iterations]);
				mBmp.SetRGBPixel(x, y, rgb.mRed, rgb.mGreen, rgb.mBlue);
			}
		}
	}
}

void FractalBmp::Generate()
{
	IterationsFirstPass();
	IterationsSecondPass();
}

bool FractalBmp::WriteToFile(const char* filename) const
{
	return mBmp.WriteToFile(filename);
}

} /* namespace mandelbrot */
} /* namespace mabz */