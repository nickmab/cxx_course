#include <cmath>
#include <cstdint>
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

FractalBmp::FractalBmp(
		double xCenter, 
		double yCenter, 
		double xDomainWidth, 
		int pixelWidth,
		int pixelHeight
	)
	: mBmp(pixelWidth, pixelHeight)
	, mIterations(pixelWidth*pixelHeight, 0)
{
	const double aspectRatio = static_cast<double>(pixelHeight) / pixelWidth;
	const double xMidptOffset = 0.5 * xDomainWidth;
	const double yMidptOffset = aspectRatio * xMidptOffset;
	
	mXScaler.reset(new mabz::ImgScaler(
		0, pixelWidth - 1, xCenter - xMidptOffset, xCenter + xMidptOffset));
	mYScaler.reset(new mabz::ImgScaler(
		0, pixelHeight - 1, yCenter - yMidptOffset, yCenter + yMidptOffset));
}

void FractalBmp::Generate()
{
	const int width = mBmp.Width();
	const int height = mBmp.Height();
	
	std::cout << "Generating mandelbrot iteration counts for " 
			  << width * height << " pixels." << std::endl;

	int pixelsInMandelbrotSet{0};
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			// determine real and imaginary parts corresponding to the pixel we're working with...
			double scaledX{0};
			double scaledY{0};
			mXScaler->Convert(x, scaledX);
			mYScaler->Convert(y, scaledY);

			// determine the mandelbrot iterations...
			int iterations = 0;
			pixelsInMandelbrotSet += !calculator::IsDivergent(iterations, scaledX, scaledY);
			mIterations[y*width + x] = iterations;
		}
	}

	std::cout << "Found " << pixelsInMandelbrotSet << " pixels in the Mandelbrot set." << std::endl;

	mHasBeenGenerated = true;
}

void FractalBmp::Colorize(const mabz::color::SingleColorScheme& colors)
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

bool FractalBmp::WriteToFile(const char* filename) const
{
	return mBmp.WriteToFile(filename);
}

FractalBmpFactory* FractalBmpFactory::NewFromPbufJsonFile(const char* filename, std::string& outErrorStr)
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
		FractalBmpFactory* factory = new FractalBmpFactory();
		fractal_proto::FractalBmpFactory config;
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
				const fractal_proto::MandelbrotBmp& bmpConfig = config.bitmaps(i);
				
				auto fractalBmp = std::make_shared<FractalBmp>(
					bmpConfig.x_center(),
					bmpConfig.y_center(),
					bmpConfig.x_domain_width(),
					bmpConfig.pixel_width(),
					bmpConfig.pixel_height());
				
				for (int j = 0; j < bmpConfig.color_schemes_size(); j++)
				{
					const fractal_proto::SingleColorScheme& colorConfig = bmpConfig.color_schemes(j);
					auto colors = std::make_unique<const mabz::color::SingleColorScheme>(
						mabz::mandelbrot::calculator::MAX_ITERATIONS,
						colorConfig.mandelbrot_color().red(),
						colorConfig.mandelbrot_color().green(),
						colorConfig.mandelbrot_color().blue(),
						colorConfig.iter_base_color().red(),
						colorConfig.iter_base_color().green(),
						colorConfig.iter_base_color().blue());
					
					auto tup = std::make_tuple(
						fractalBmp, 
						std::move(colors),
						std::move(std::string(colorConfig.out_filename())));

					factory->mBmps.push_back(std::move(tup));
				}

			}
			return factory;
		}
	}
}

void FractalBmpFactory::Run()
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