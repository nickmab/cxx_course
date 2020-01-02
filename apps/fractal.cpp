#include <iostream>

#include "advanced_cxx/bmp.h"

int main()
{
	const int width = 800;
	const int height = 800;
	mabz::Bmp bmp(width, height);

	for (int x = 0; x < 2*width; x++)
	{
		for (int y = 0; y < 2*height; y++)
		{
			bmp.SetRGBPixel(x, y, 255, 0, 0);
		}	
	}

	std::cout << bmp.WriteToFile("very_simple.bmp") << std::endl;
	return 0;
}