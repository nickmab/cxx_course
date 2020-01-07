#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include <advanced_cxx/bmp.h>
#include <advanced_cxx/color_utils.h>
#include <advanced_cxx/fractal.pb.h>

namespace mabz { namespace mandelbrot {

// abstract base class for going from a "score" for a pixel (e.g. something
// representing what intensity/style of color it should have) to an actual RGB.
class ColorScheme
{
public:
	virtual mabz::color::RGB GetColor(int score) const = 0;
	virtual ~ColorScheme() {}
};

// This was developed for colorizing the mandelbrot set, where you have
// one color for a "maxed out" score (i.e. if it's in the set) and 
// a color scale if it's not in the set, which depends on how quickly it diverges.
#define ui8 std::uint8_t
class SingleColorScheme : public ColorScheme
{
private:
	int mScalingDenominator{0};
	mabz::color::RGB mHundredPerCentColor;
	mabz::color::HSV mScalingBaseColor;

public:
	// mScalingDenominator, RGB of mHundredPerCentColor, RGB of mScalingBaseColor.
	SingleColorScheme(int, ui8, ui8, ui8, ui8, ui8, ui8);
	
	virtual mabz::color::RGB GetColor(int score) const override;
};
#undef ui8

class DodgyColorScheme : public ColorScheme
{
public:
	// just to check the vtable works ok...
	virtual mabz::color::RGB GetColor(int) const override
	{
		return std::move(mabz::color::RGB(30, 40, 50));
	}
};

class FractalBmp
{
private:
	mabz::Bmp mBmp;
	const mabz::PixelXYMapper mPixelXYMapper;

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
	void Colorize(const ColorScheme&);
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
		std::unique_ptr<const ColorScheme>, 
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
