#include <iostream>

#include "advanced_cxx/bmp.h"

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

	std::cout << bmp.WriteToFile("very_simple.bmp") << std::endl;
	return 0;
}