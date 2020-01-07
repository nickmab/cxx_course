#pragma once

#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include <advanced_cxx/bmp.h>
#include <advanced_cxx/color_utils.h>
#include <advanced_cxx/fractal.pb.h>

namespace mabz { namespace mandelbrot {

class FractalBmp
{
private:
	mabz::Bmp mBmp;
	mabz::PixelXYMapper mPixelXYMapper;

	std::vector<int> mIterations;

	bool mHasBeenGenerated{false};

public:
	FractalBmp(
		double xCenter, 
		double yCenter, 
		double xDomainWidth, 
		int pixelWidth,
		int pixelHeight)

		: mBmp(pixelWidth, pixelHeight)
		, mPixelXYMapper(xCenter, yCenter, xDomainWidth, pixelWidth, pixelHeight)
		, mIterations(pixelWidth*pixelHeight, 0)
	{}
	FractalBmp(const FractalBmp&) = delete;

	bool HasBeenGenerated() const { return mHasBeenGenerated; }
	void Generate();
	void Colorize(const mabz::color::SingleColorScheme&);
	bool WriteToFile(const char* filename) const;
};

class FractalBmpFactory
{
private:
	// Pre-cached runs to generate fractals (just add water by calling the combos of args).
	// Note that FractalBmp instances can be re-used, hence the unique ptr.
	// the string represents the output filename of the bmp.
	std::vector<
		std::tuple< std::shared_ptr<FractalBmp>, 
		std::unique_ptr<const mabz::color::SingleColorScheme>, 
		const std::string> > mBmps;
	
	// must use a static method to create. Constructor is private.
	FractalBmpFactory() {}

public:
	static FractalBmpFactory* NewFromPbufJsonFile(const char* filename, std::string& outErrorStr);

	// I am really not doing much error checking at the moment lol.
	void Run();
};

} /* namespace mandelbrot */
} /* namespace mabz */
