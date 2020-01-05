#include <iostream>

#include "advanced_cxx/fractal.h"

int main()
{
	mabz::mandelbrot::FractalBmp frac(-1.0, 0.2, 1.25, 800, 600);
	frac.Generate();
	frac.Colorize();
	std::cout << frac.WriteToFile("very_simple.bmp") << std::endl;
	
	return 0;
}