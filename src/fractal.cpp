#include <cmath>
#include <cstdint>
#include <iostream>

#include "advanced_cxx/fractal.h"

#include "advanced_cxx/color_utils.h"
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
{
	const double aspectRatio = static_cast<double>(pixelHeight) / pixelWidth;
	const double xMidptOffset = 0.5 * xDomainWidth;
	const double yMidptOffset = aspectRatio * xMidptOffset;
	
	mXScaler.reset(new mabz::ImgScaler(
		0, pixelWidth - 1, xCenter - xMidptOffset, xCenter + xMidptOffset));
	mYScaler.reset(new mabz::ImgScaler(
		0, pixelHeight - 1, yCenter - yMidptOffset, yCenter + yMidptOffset));
}

void FractalBmp::Generate()
{
	const mabz::color::RGB baseColor(10, 150, 20);
	const int width = mBmp.Width();
	const int height = mBmp.Height();
	
	std::cout << "Generating mandelbrot bmp for " 
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
				const double intensity = sqrt(static_cast<double>(iterations) / mabz::mandelbrot::calculator::MAX_ITERATIONS);
				mabz::color::HSV hsv = baseColor.ToHSV();
				hsv.mHue *= intensity;
				const mabz::color::RGB rgb = hsv.ToRGB();
				mBmp.SetRGBPixel(x, y, rgb.mRed, rgb.mGreen, rgb.mBlue);
			}
			else
			{
				pixelsInMandelbrotSet++;
				mBmp.SetRGBPixel(x, y, 80, 10, 80); 
			}
		}
	}

	std::cout << "Found " << pixelsInMandelbrotSet << " pixels in the Mandelbrot set." << std::endl;
}

bool FractalBmp::WriteToFile(const char* filename) const
{
	return mBmp.WriteToFile(filename);
}

} /* namespace mandelbrot */
} /* namespace mabz */