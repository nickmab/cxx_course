#include <iostream>

#include "advanced_cxx/bmp.h"

int main()
{
	mabz::Bmp bmp(800, 600);
	std::cout << bmp.WriteToFile("very_simple.bmp") << std::endl;
	return 0;
}