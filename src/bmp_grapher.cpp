#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

#include "advanced_cxx/bmp_grapher.h"

#include "advanced_cxx/graphers/mandelbrot.h"
#include "advanced_cxx/colors/color_utils.h"
#include "advanced_cxx/bmp_grapher.pb.h"

#include <google/protobuf/util/json_util.h>

namespace mabz { namespace graphers {

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
				std::shared_ptr<BmpGrapher> bmpGrapher{nullptr};

				if (bmpConfig.has_mandelbrot_calc())
				{
					bmpGrapher.reset(new mabz::graphers::MandelbrotCalc(
						bmpConfig.x_center(),
						bmpConfig.y_center(),
						bmpConfig.x_domain_width(),
						bmpConfig.pixel_width(),
						bmpConfig.pixel_height(),
						bmpConfig.mandelbrot_calc().max_iterations()));
				}
				else
				{
					delete factory;
					outErrorStr = "Must specify a calculation method in BmpGrapher config!";
					return nullptr;
				}

				for (int j = 0; j < bmpConfig.color_configs_size(); j++)
				{			
					const bmp_grapher_proto::ColorConfig& colorConfig = bmpConfig.color_configs(j);

					std::unique_ptr<const BmpGrapher::RunArgs> runArgs{nullptr};
					if (colorConfig.has_single_color_scheme())
					{
						auto csdata = colorConfig.single_color_scheme();
						runArgs.reset(new mabz::graphers::MandelbrotCalc::RunSingleColorSchemeArgs(
							std::string(colorConfig.out_filename()),
							csdata.hundred_per_cent_color().red(),
							csdata.hundred_per_cent_color().green(),
							csdata.hundred_per_cent_color().blue(),
							csdata.scaling_base_color().red(),
							csdata.scaling_base_color().green(),
							csdata.scaling_base_color().blue()));
					}
					else if (colorConfig.has_dodgy_color_scheme())
					{
						delete factory;
						outErrorStr = "Dodgy color scheme currently unsupported!";
						return nullptr;
					}
					else
					{
						delete factory;
						outErrorStr = "Must specify a color scheme in ColorConfig config!";
						return nullptr;
					}
					
					factory->mPendingGraphs.push_back(std::make_pair(bmpGrapher, std::move(runArgs)));
				}
			}
			return factory;
		}
	}
}

void BmpGrapherFactory::Run()
{
	for (auto& t : mPendingGraphs)
	{
		t.first->Run(t.second.get());
	}
}

} /* namespace graphers */
} /* namespace mabz */