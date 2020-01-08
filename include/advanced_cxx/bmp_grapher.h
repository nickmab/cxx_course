#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include <advanced_cxx/bmp.h>
#include <advanced_cxx/calculators/pixel_score_calculator.h>
#include <advanced_cxx/colors/color_scheme.h>
#include <advanced_cxx/bmp_grapher.pb.h>

namespace mabz {

class BmpGrapher
{
private:
	mabz::Bmp mBmp;
	const mabz::PixelXYMapper mPixelXYMapper;

	// each pixel gets a "score" (e.g. result from calling a function with
	// arguments X,Y corresponding to the values of the pixels) representing
	// its color intensity (for example).
	std::vector<int> mPixelScores;

	bool mColorScoresGenerated{false};

public:
	BmpGrapher(
		double xCenter, 
		double yCenter, 
		double xDomainWidth, 
		int pixelWidth,
		int pixelHeight)

		: mBmp(pixelWidth, pixelHeight)
		, mPixelXYMapper(xCenter, yCenter, xDomainWidth, pixelWidth, pixelHeight)
		, mPixelScores(pixelWidth*pixelHeight, 0)
	{}
	BmpGrapher(const BmpGrapher&) = delete;

	bool ColorScoresGenerated() const { return mColorScoresGenerated; }
	void GenerateColorScores(const calculators::PixelScoreCalculator&);
	void Colorize(const color::ColorScheme&);
	bool WriteToFile(const char* filename) const;
};

class BmpGrapherFactory
{
private:
	// Pre-cached runs to generate fractals (just add water by calling the combos of args).
	// Note that BmpGrapher instances can be re-used, hence the unique ptr.
	// the string represents the output filename of the bmp.
	std::vector<
		std::tuple< std::shared_ptr<BmpGrapher>, 
		std::shared_ptr<const calculators::PixelScoreCalculator>, 
		std::unique_ptr<const color::ColorScheme>, 
		const std::string> > mBmps;
	
	// must use a static method to create. Constructor is private.
	BmpGrapherFactory() {}

public:
	static BmpGrapherFactory* NewFromPbufJsonFile(const char* filename, std::string& outErrorStr);

	// I am really not doing much error checking at the moment lol.
	void Run();
};

} /* namespace mabz */
