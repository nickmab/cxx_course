#include <cstdint>
#include <iostream>

#include "advanced_cxx/bmp.h"
#include "advanced_cxx/color_map.h"
#include "advanced_cxx/img_scaler.h"

int main()
{
	const int width = 800;
	const int height = 600;
	mabz::Bmp bmp(width, height);

	for (int x = 0; x < 0.5*width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			bmp.SetRGBPixel(x, y, 255, 0, 0);
		}	
	}

	for (int x = 0.5*width; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			bmp.SetRGBPixel(x, y, 0, 0, 255);
		}	
	}

	const mabz::ImgScaler scaler(-1.0, 1.0, 0, width-1);
	int scaledX = 0;
	int maxScaledX = -9999999;
	int minScaledX = 9999999;
	for (double x = -1.0; x < 1.0; x += 0.1)
	{
		scaler.Convert(x, scaledX);
		if (scaledX > maxScaledX) maxScaledX = scaledX;
		if (scaledX < minScaledX) minScaledX = scaledX;
	}

	mabz::color::BasicTable colors(4);
	colors.AddEntry(10, 120, 40, 40);
	colors.AddEntry(50, 120, 120, 40);
	colors.AddEntry(125, 120, 200, 60);
	colors.AddEntry(750, 230, 230, 230);

	std::cout << bmp.WriteToFile("very_simple.bmp") << std::endl;
	std::cout << minScaledX << ", " << maxScaledX << std::endl;
	std::cout << colors << std::endl;

	std::uint8_t r{0};
	std::uint8_t g{0};
	std::uint8_t b{0};
	std::cout << "(" << colors.GetColor(0, r, g, b) << ") (" 
		<< (int)r << ") (" << (int)g << ") (" << (int)b << ")" << std::endl;
	std::cout << "(" << colors.GetColor(49, r, g, b) << ") (" 
		<< (int)r << ") (" << (int)g << ") (" << (int)b << ")" << std::endl;
	std::cout << "(" << colors.GetColor(900, r, g, b) << ") (" 
		<< (int)r << ") (" << (int)g << ") (" << (int)b << ")" << std::endl;
	return 0;
}