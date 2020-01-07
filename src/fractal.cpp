#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

#include "advanced_cxx/fractal.h"

#include "advanced_cxx/color_utils.h"
#include "advanced_cxx/fractal.pb.h"
#include "advanced_cxx/mandelbrot.h"

#include <google/protobuf/util/json_util.h>

namespace mabz { namespace mandelbrot {

#define ui8 std::uint8_t
SingleColorScheme::SingleColorScheme(
	int scalingDenominator, 
	ui8 rFull, ui8 gFull, ui8 bFull, 
	ui8 rBase, ui8 gBase, ui8 bBase)
	
	: mScalingDenominator(scalingDenominator)
	, mHundredPerCentColor(rFull, gFull, bFull)
	, mScalingBaseColor(std::move(mabz::color::RGB::ToHSV(rBase, gBase, bBase)))
{}
#undef ui8

mabz::color::RGB SingleColorScheme::GetColor(int score) const
{
	if (score == mScalingDenominator)
	{
		// makes / returns a copy obviously.
		return mHundredPerCentColor;
	}
	else
	{
		const double intensity = sqrt(static_cast<double>(score) / mScalingDenominator);
		mabz::color::HSV hsv(mScalingBaseColor);
		hsv.mHue *= intensity;
		hsv.mSaturation *= intensity;
		hsv.mValue *= intensity;
		return std::move(hsv.ToRGB());
	}
}

void BmpGrapher::Generate()
{
	const int width = mBmp.Width();
	const int height = mBmp.Height();
	
	std::cout << "Generating pixel scores for " 
			  << width * height << " pixels." << std::endl;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			double scaledX{0};
			double scaledY{0};
			mPixelXYMapper.Convert(x, y, scaledX, scaledY);

			int iterations = 0;
			calculator::IsDivergent(iterations, scaledX, scaledY);
			mIterations[y*width + x] = iterations;
		}
	}

	mHasBeenGenerated = true;
}

void BmpGrapher::Colorize(const ColorScheme& colors)
{
	const int width = mBmp.Width();
	const int height = mBmp.Height();
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			const mabz::color::RGB rgb = colors.GetColor(mIterations[y*width + x]);
			mBmp.SetRGBPixel(x, y, rgb.mRed, rgb.mGreen, rgb.mBlue);
		}
	}
}

bool BmpGrapher::WriteToFile(const char* filename) const
{
	return mBmp.WriteToFile(filename);
}

BmpGrapherFactory* BmpGrapherFactory::NewFromPbufJsonFile(const char* filename, std::string& outErrorStr)
{
	using namespace google::protobuf::util;

	std::stringstream ss;
	std::ifstream inFile(filename);
	if (!inFile.is_open())
	{
		ss << "Could not open file " << filename << "!";
		outErrorStr = ss.str();
		return nullptr;
	}

	ss << inFile.rdbuf();
	inFile.close();
	if (!inFile.good())
	{
		// clear the stringstream of any junk first...
		ss.str(std::string());		
		ss << "Error reading from file " << filename << "!";
		outErrorStr = ss.str();
		return nullptr;
	}
	else
	{
		BmpGrapherFactory* factory = new BmpGrapherFactory();
		fractal_proto::BmpGrapherFactory config;
		Status status = JsonStringToMessage(ss.str(), &config);
		// Status status = JsonStringToMessage("buttocks", &config);
		
		if (!status.ok())
		{
			delete factory;
			outErrorStr = status.ToString();
			return nullptr;
		}
		else
		{
			for (int i = 0; i < config.bitmaps_size(); ++i)
			{
				const fractal_proto::BmpGrapher& bmpConfig = config.bitmaps(i);
				
				auto bmpGrapher = std::make_shared<BmpGrapher>(
					bmpConfig.x_center(),
					bmpConfig.y_center(),
					bmpConfig.x_domain_width(),
					bmpConfig.pixel_width(),
					bmpConfig.pixel_height());
				
				for (int j = 0; j < bmpConfig.color_schemes_size(); j++)
				{
					const fractal_proto::SingleColorScheme& colorConfig = bmpConfig.color_schemes(j);
					auto colors = std::make_unique<const SingleColorScheme>(
						mabz::mandelbrot::calculator::MAX_ITERATIONS,
						colorConfig.mandelbrot_color().red(),
						colorConfig.mandelbrot_color().green(),
						colorConfig.mandelbrot_color().blue(),
						colorConfig.iter_base_color().red(),
						colorConfig.iter_base_color().green(),
						colorConfig.iter_base_color().blue());
					
					auto tup = std::make_tuple(
						bmpGrapher, 
						std::move(colors),
						std::move(std::string(colorConfig.out_filename())));

					factory->mBmps.push_back(std::move(tup));
				}

				// make a dodgy one just to test the code out...
				auto colors = std::make_unique<const DodgyColorScheme>();
				auto tup = std::make_tuple(
					bmpGrapher, 
					std::move(colors),
					std::move(std::string("flat_color.bmp")));

				factory->mBmps.push_back(std::move(tup));
			}
			return factory;
		}
	}
}

void BmpGrapherFactory::Run()
{
	for (auto& t : mBmps)
	{
		auto& frac = std::get<0>(t);
		auto& colors = std::get<1>(t);
		auto& filename = std::get<2>(t);

		if (!frac->HasBeenGenerated())
		{
			frac->Generate();
		}

		frac->Colorize(*colors);
		frac->WriteToFile(filename.c_str());
	}
}

} /* namespace mandelbrot */
} /* namespace mabz */