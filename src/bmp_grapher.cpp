#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>

#include "advanced_cxx/bmp_grapher.h"

#include "advanced_cxx/graphers/mandelbrot.h"
#include "advanced_cxx/graphers/quadratic.h"
#include "advanced_cxx/bmp_grapher.pb.h"

#include <google/protobuf/util/json_util.h>

namespace mabz { namespace graphers {

namespace nsmandel = mabz::graphers::mandelbrot;
namespace nsquadratic = mabz::graphers::quadratic;

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
				
				if (bmpConfig.has_mandelbrot_config())
				{
					bmpGrapher.reset(new nsmandel::MandelbrotCalc(
						bmpConfig.x_center(),
						bmpConfig.y_center(),
						bmpConfig.x_domain_width(),
						bmpConfig.pixel_width(),
						bmpConfig.pixel_height(),
						bmpConfig.mandelbrot_config().max_iterations()));

					auto mbConfig = bmpConfig.mandelbrot_config();
					for (int j = 0; j < mbConfig.run_args_size(); j++)
					{
						auto runConfig = mbConfig.run_args(j);
						std::shared_ptr<const BmpGrapher::RunArgs> bmpRunArgs{nullptr};
						std::shared_ptr<const BmpGrapher::RunArgs> colorRunArgs{nullptr};
						nsmandel::ColorScheme colorSchemeName;
						if (runConfig.has_single_color_scheme_args())
						{
							colorSchemeName = nsmandel::ColorScheme::SINGLE;
							
							auto csdata = runConfig.single_color_scheme_args();
							colorRunArgs.reset(new nsmandel::SingleColorScheme::ConstructorArgs(
								csdata.mandelbrot_color().red(),
								csdata.mandelbrot_color().green(),
								csdata.mandelbrot_color().blue(),
								csdata.base_color().red(),
								csdata.base_color().green(),
								csdata.base_color().blue()));
						}
						else
						{
							delete factory;
							outErrorStr = "Must specify a color scheme in Mandelbrot config!";
							return nullptr;
						}

						bmpRunArgs.reset(new nsmandel::RunArgs(
							std::string(runConfig.out_filename()),
							colorSchemeName,
							colorRunArgs));

						RunPair pair{bmpGrapher, bmpRunArgs};
						factory->mPendingGraphs.push_back(std::move(pair));
					}
				}
				else if (bmpConfig.has_quadratic_config())
				{
					auto qConfig = bmpConfig.quadratic_config();

					bmpGrapher.reset(new nsquadratic::QuadraticCalc(
						bmpConfig.x_center(),
						bmpConfig.y_center(),
						bmpConfig.x_domain_width(),
						bmpConfig.pixel_width(),
						bmpConfig.pixel_height(),
						qConfig.param_a(),
						qConfig.param_b(),
						qConfig.param_c()));

					for (int j = 0; j < qConfig.run_args_size(); j++)
					{
						auto runConfig = qConfig.run_args(j);
						std::shared_ptr<const BmpGrapher::RunArgs> bmpRunArgs{nullptr};
						std::shared_ptr<const BmpGrapher::RunArgs> colorRunArgs{nullptr};
						nsquadratic::ColorScheme colorSchemeName;
						if (runConfig.has_dual_color_scheme_args())
						{
							colorSchemeName = nsquadratic::ColorScheme::DUAL;
							
							auto csdata = runConfig.dual_color_scheme_args();
							colorRunArgs.reset(new nsquadratic::DualColorScheme::ConstructorArgs(
								csdata.max_distance(),
								csdata.background_color().red(),
								csdata.background_color().green(),
								csdata.background_color().blue(),
								csdata.curve_color().red(),
								csdata.curve_color().green(),
								csdata.curve_color().blue()));
						}
						else
						{
							delete factory;
							outErrorStr = "Must specify a color scheme in Quadratic config!";
							return nullptr;
						}

						bmpRunArgs.reset(new nsquadratic::RunArgs(
							std::string(runConfig.out_filename()),
							colorSchemeName,
							colorRunArgs));

						RunPair pair{bmpGrapher, bmpRunArgs};
						factory->mPendingGraphs.push_back(std::move(pair));
					}
				}
				else
				{
					delete factory;
					outErrorStr = "Must specify a grapher (e.g. Mandelbrot or Quadratic) BmpGrapher config!";
					return nullptr;
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
		t.mGrapher->Run(t.mRunArgs);
	}
}

} /* namespace graphers */
} /* namespace mabz */