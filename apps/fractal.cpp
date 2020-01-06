#include <iostream>

#include "advanced_cxx/fractal.h"

int main()
{
	mabz::mandelbrot::FractalBmp frac(-0.5, 0.5, 1.0, 600, 800);
	frac.Generate();
	std::cout << frac.WriteToFile("very_simple.bmp") << std::endl;
	
	return 0;
}