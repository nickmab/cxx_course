#include <iostream>

#include "advanced_cxx/fractal.h"

int main()
{
	mabz::mandelbrot::FractalBmp frac(-0.7, 0.0, 3.0, 800, 600);
	std::cout << frac.Generate() << std::endl;
	std::cout << frac.WriteToFile("very_simple.bmp") << std::endl;
	
	return 0;
}