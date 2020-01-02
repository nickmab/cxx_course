#include <iostream>

#include "advanced_cxx/bmp.h"
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

	std::cout << bmp.WriteToFile("very_simple.bmp") << std::endl;
	std::cout << minScaledX << ", " << maxScaledX << std::endl;
	return 0;
}