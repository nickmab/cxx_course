#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include <advanced_cxx/bmp.h>
#include <advanced_cxx/colors/color_scheme.h>
#include <advanced_cxx/bmp_grapher.pb.h>

namespace mabz { namespace graphers {

class BmpGrapher
{
protected:
	mabz::Bmp mBmp;
	const mabz::PixelXYMapper mPixelXYMapper;

public:
	BmpGrapher(
		double xCenter, 
		double yCenter, 
		double xDomainWidth, 
		int pixelWidth,
		int pixelHeight)

		: mBmp(pixelWidth, pixelHeight)
		, mPixelXYMapper(xCenter, yCenter, xDomainWidth, pixelWidth, pixelHeight)
	{}
	BmpGrapher(const BmpGrapher&) = delete;
	virtual ~BmpGrapher() {}

	class RunArgs
	{
	public:
		virtual ~RunArgs() {}
	};
	virtual void Run(const BmpGrapher::RunArgs*) = 0;

	bool WriteToFile(const char* filename) const;
};

class BmpGrapherFactory
{
private:
	// Pre-cached runs to generate fractals (just add water by calling the combos of args).
	// Note that BmpGrapher instances can be re-used, hence the unique ptr.
	// the string represents the output filename of the bmp.
	std::vector<std::pair<std::shared_ptr<BmpGrapher>,std::unique_ptr<const BmpGrapher::RunArgs> > > mPendingGraphs;
	
	// must use a static method to create. Constructor is private.
	BmpGrapherFactory() {}

public:
	static BmpGrapherFactory* NewFromPbufJsonFile(const char* filename, std::string& outErrorStr);

	// I am really not doing much error checking at the moment lol.
	void Run();
};

} /* namespace graphers */
} /* namespace mabz */
