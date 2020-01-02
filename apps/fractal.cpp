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
	colors.AddEntry(10, 20, 20, 80);
	colors.AddEntry(50, 60, 20, 120);
	colors.AddEntry(125, 150, 60, 120);
	colors.AddEntry(250, 60, 150, 60);
	colors.AddEntry(500, 120, 200, 60);
	colors.AddEntry(750, 140, 200, 150);
	colors.AddEntry(1000, 200, 200, 200);
	colors.AddEntry(2500, 245, 245, 245);
	std::cout << colors << std::endl;

	// project from the -1:1 domain and range to the width:height domain and range.
	const mabz::ImgScaler xScaler(-1.0, 1.0, 0, bmp.Width()-1);
	const mabz::ImgScaler yScaler(-1.0, 1.0, 0, bmp.Height()-1);
	
	for (double x = -1.0; x < 1.0; x += 0.005)
	{
		for (double y = -1.0; y < 1.0; y += 0.005)
		{
			// determine which pixel we're working with...
			int scaledX = 0;
			int scaledY = 0;
			xScaler.Convert(x, scaledX);
			yScaler.Convert(y, scaledY);

			// determine the mandelbrot iterations...
			unsigned iterations = 0;
			mabz::mandelbrot::calculator::IsDivergent(iterations, x, y);
			std::uint8_t r = 0;
			std::uint8_t g = 0;
			std::uint8_t b = 0;
			colors.GetColor(iterations, r, g, b);
			bmp.SetRGBPixel(scaledX, scaledY, r, g, b);
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