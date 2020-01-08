#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

#include "advanced_cxx/bmp_grapher.h"

#include "advanced_cxx/calculators/mandelbrot.h"
#include "advanced_cxx/colors/color_utils.h"
#include "advanced_cxx/bmp_grapher.pb.h"

#include <google/protobuf/util/json_util.h>

namespace mabz {

void BmpGrapher::GenerateColorScores(const calculators::PixelScoreCalculator& calc)
{
	const int width = mBmp.Width();
	const int height = mBmp.Height();
	
	std::cout << "Generating pixel color scores for " 
			  << width * height << " pixels." << std::endl;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			double scaledX{0};
			double scaledY{0};
			mPixelXYMapper.Convert(x, y, scaledX, scaledY);

			int iterations = 0;
			calc.GetPixelScore(iterations, scaledX, scaledY);
			mPixelScores[y*width + x] = iterations;
		}
	}

	mColorScoresGenerated = true;
}

void BmpGrapher::Colorize(const color::ColorScheme& colors)
{
	const int width = mBmp.Width();
	const int height = mBmp.Height();
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			const color::RGB rgb = colors.GetColor(mPixelScores[y*width + x]);
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
		bmp_grapher_proto::BmpGrapherFactory config;
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
				const bmp_grapher_proto::BmpGrapher& bmpConfig = config.bitmaps(i);
				
				std::shared_ptr<const calculators::PixelScoreCalculator> calc{nullptr};
				if (bmpConfig.has_mandelbrot_calc())
				{
					calc.reset(new calculators::MandelbrotCalc(bmpConfig.mandelbrot_calc().max_iterations()));
				}
				else
				{
					delete factory;
					outErrorStr = "Must specify a calculation method in BmpGrapher config!";
					return nullptr;
				}

				auto bmpGrapher = std::make_shared<BmpGrapher>(
					bmpConfig.x_center(),
					bmpConfig.y_center(),
					bmpConfig.x_domain_width(),
					bmpConfig.pixel_width(),
					bmpConfig.pixel_height());
				
				for (int j = 0; j < bmpConfig.color_configs_size(); j++)
				{			
					const bmp_grapher_proto::ColorConfig& colorConfig = bmpConfig.color_configs(j);

					std::unique_ptr<const color::ColorScheme> colors{nullptr};
					if (colorConfig.has_single_color_scheme())
					{
						auto csdata = colorConfig.single_color_scheme();
						colors.reset(new color::SingleColorScheme(
							csdata.scaling_denominator(),
							csdata.hundred_per_cent_color().red(),
							csdata.hundred_per_cent_color().green(),
							csdata.hundred_per_cent_color().blue(),
							csdata.scaling_base_color().red(),
							csdata.scaling_base_color().green(),
							csdata.scaling_base_color().blue()));
					}
					else if (colorConfig.has_dodgy_color_scheme())
					{
						auto csdata = colorConfig.dodgy_color_scheme();
						colors.reset(new color::DodgyColorScheme(
							csdata.solid_color().red(),
							csdata.solid_color().green(),
							csdata.solid_color().blue()));
					}
					else
					{
						delete factory;
						outErrorStr = "Must specify a color scheme in ColorConfig config!";
						return nullptr;
					}
					
					auto tup = std::make_tuple(
						bmpGrapher, 
						calc,
						std::move(colors),
						std::move(std::string(colorConfig.out_filename())));

					factory->mBmps.push_back(std::move(tup));
				}
			}
			return factory;
		}
	}
}

void BmpGrapherFactory::Run()
{
	for (auto& t : mBmps)
	{
		auto& grapher = std::get<0>(t);
		auto& colors = std::get<2>(t);
		auto& filename = std::get<3>(t);

		if (!grapher->ColorScoresGenerated())
		{
			auto& calc = std::get<1>(t);
			grapher->GenerateColorScores(*calc);
		}

		grapher->Colorize(*colors);
		grapher->WriteToFile(filename.c_str());
	}
}

} /* namespace mabz */