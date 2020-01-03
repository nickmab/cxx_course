#include <cstdint>
#include <iostream>

#include "advanced_cxx/bmp.h"
#include "advanced_cxx/color_map.h"
#include "advanced_cxx/img_scaler.h"
#include "advanced_cxx/mandelbrot.h"

// true or false indicates success or failure...
// although at the moment I am not doing proper error checking, basically just returning true lol.
bool fillBmpWithColor(mabz::Bmp& bmp)
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
	std::cout << colors << std::endl;

	// project from the -1:1 domain and range to the width:height domain and range.
	// maintaining the same aspect ratio.
	const double cartesianXMin{-2.2};
	const double cartesianXMax{0.9};
	const double aspectRatio = (double)bmp.Height() / bmp.Width();
	const double cartesianYMidpt{0.0};
	const double cartesianYMidptOffset = 0.5 * aspectRatio * (cartesianXMax - cartesianXMin);
	const double cartesianYMin{cartesianYMidpt - cartesianYMidptOffset};
	const double cartesianYMax{cartesianYMidpt + cartesianYMidptOffset};
	const mabz::ImgScaler xScaler(0, bmp.Width()-1, cartesianXMin, cartesianXMax);
	const mabz::ImgScaler yScaler(0, bmp.Height()-1, cartesianYMin, cartesianYMax);
	
	for (int x = 0; x < bmp.Width(); ++x)
	{
		for (int y = 0; y < bmp.Height(); ++y)
		{
			// determine which pixel we're working with...
			double scaledX{0};
			double scaledY{0};
			xScaler.Convert(x, scaledX);
			yScaler.Convert(y, scaledY);

			// determine the mandelbrot iterations...
			unsigned iterations = 0;
			mabz::mandelbrot::calculator::IsDivergent(iterations, scaledX, scaledY);
			std::uint8_t r = 0;
			std::uint8_t g = 0;
			std::uint8_t b = 0;
			colors.GetColor(iterations, r, g, b);
			bmp.SetRGBPixel(x, y, r, g, b);
		}
	}

	return true;
}

int main()
{
	const int width = 800;
	const int height = 600;
	mabz::Bmp bmp(width, height);
	fillBmpWithColor(bmp);
	std::cout << bmp.WriteToFile("very_simple.bmp") << std::endl;
	
	return 0;
}