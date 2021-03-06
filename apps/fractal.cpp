#include <iostream>
#include <string>

#include "advanced_cxx/bmp_grapher.h"

int main(int argc, char* argv[])
{

	if (argc != 2)
	{
		std::cout << "Must provide a config json file as the lone cmd line arg." << std::endl;
		return 1;
	}
	
	const char* configFilename = argv[1];
	std::cout << "Will attempt to load config from file: " << configFilename << std::endl; 
	
	std::string s;
	auto ptr = mabz::graphers::BmpGrapherFactory::NewFromPbufJsonFile(configFilename, s);
	
	if (ptr == nullptr)
	{
		std::cout << "There was an error, sorry!" << std::endl;
		std::cout << s << std::endl;
		return 1;
	}

	ptr->Run();

	return 0;
}