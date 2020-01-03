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
	, mXMin(xCenter - 0.5*xDomainWidth)
	, mXMax(xCenter + 0.5*xDomainWidth)
{
	const double aspectRatio = static_cast<double>(pixelHeight) / pixelWidth;
	const double yMidptOffset = 0.5*aspectRatio*xDomainWidth;
	mYMin = yCenter - yMidptOffset;
	mYMax = yCenter + yMidptOffset;
	mXScaler.reset(new mabz::ImgScaler(0, pixelWidth - 1, mXMin, mXMax));
	mYScaler.reset(new mabz::ImgScaler(0, pixelHeight - 1, mYMin, mYMax));
}

bool FractalBmp::Generate()
{
	mabz::color::BasicTable colors(8);
	colors.AddEntry(3, 20, 20, 80);
	colors.AddEntry(6, 60, 20, 120);
	colors.AddEntry(9, 150, 60, 120);
	colors.AddEntry(20, 60, 150, 60);
	colors.AddEntry(50, 120, 200, 60);
	colors.AddEntry(100, 140, 200, 150);
	colors.AddEntry(500, 20, 200, 40);
	colors.AddEntry(1500, 200, 200, 200);

	for (int x = 0; x < mBmp.Width(); ++x)
	{
		for (int y = 0; y < mBmp.Height(); ++y)
		{
			// determine which pixel we're working with...
			double scaledX{0};
			double scaledY{0};
			mXScaler->Convert(x, scaledX);
			mYScaler->Convert(y, scaledY);

			// determine the mandelbrot iterations...
			unsigned iterations = 0;
			calculator::IsDivergent(iterations, scaledX, scaledY);
			std::uint8_t r = 0;
			std::uint8_t g = 0;
			std::uint8_t b = 0;
			colors.GetColor(iterations, r, g, b);
			mBmp.SetRGBPixel(x, y, r, g, b);
		}
	}

	return true;
}

bool FractalBmp::WriteToFile(const char* filename) const
{
	return mBmp.WriteToFile(filename);
}

} /* namespace mandelbrot */
} /* namespace mabz */