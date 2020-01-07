#include <fstream>
#include <iostream>
#include <string>
#include <utility>

#include <google/protobuf/util/json_util.h>

#include "advanced_cxx/testproto.h"

namespace mabz {

fractal_proto::RGB protoget()
{
	fractal_proto::RGB rgb;
	rgb.set_red(1);
	rgb.set_green(2);
	rgb.set_blue(3);
	return std::move(rgb);
}

void strungout(const char* outfilename, fractal_proto::FractalBmpFactory& factoryOut)
{
	factoryOut.clear_bitmaps();
	
	{
		fractal_proto::MandelbrotBmp* bmp = factoryOut.add_bitmaps();
		bmp->set_x_center(-1.0);
		bmp->set_y_center(0.0);
		bmp->set_x_domain_width(3.0);
		bmp->set_pixel_width(800);
		bmp->set_pixel_height(600);

		{
			fractal_proto::SingleColorScheme* cs = bmp->add_color_schemes();
			
			// note that the mandelbrot colour will just default to all zeros anyway
			fractal_proto::RGB* rgb = cs->mutable_iter_base_color();
			rgb->set_red(240);
			rgb->set_green(240);
			rgb->set_blue(240);
			cs->set_out_filename("black_and_white.bmp");
		}

		{
			fractal_proto::SingleColorScheme* cs = bmp->add_color_schemes();
			
			fractal_proto::RGB* rgbMandel = cs->mutable_mandelbrot_color();
			// note that the mandelbrot colour will just default to all zeros anyway
			rgbMandel->set_blue(40);

			fractal_proto::RGB* rgb = cs->mutable_iter_base_color();
			rgb->set_red(240);
			rgb->set_green(0);
			rgb->set_blue(30);
			cs->set_out_filename("red_and_blue.bmp");
		}
	}

	{
		fractal_proto::MandelbrotBmp* bmp = factoryOut.add_bitmaps();
		bmp->set_x_center(-0.5);
		bmp->set_y_center(0.5);
		bmp->set_x_domain_width(1.0);
		bmp->set_pixel_width(600);
		bmp->set_pixel_height(800);

		{
			fractal_proto::SingleColorScheme* cs = bmp->add_color_schemes();
			
			// note that the mandelbrot colour will just default to all zeros anyway
			fractal_proto::RGB* rgb = cs->mutable_iter_base_color();
			rgb->set_red(240);
			rgb->set_green(240);
			rgb->set_blue(240);
			cs->set_out_filename("black_and_white_2.bmp");
		}

		{
			fractal_proto::SingleColorScheme* cs = bmp->add_color_schemes();
			
			fractal_proto::RGB* rgbMandel = cs->mutable_mandelbrot_color();
			// note that the mandelbrot colour will just default to all zeros anyway
			rgbMandel->set_blue(40);

			fractal_proto::RGB* rgb = cs->mutable_iter_base_color();
			rgb->set_red(240);
			rgb->set_green(0);
			rgb->set_blue(30);
			cs->set_out_filename("red_and_blue_2.bmp");
		}
	}

	std::string s;
	google::protobuf::util::JsonPrintOptions options;
	options.add_whitespace = true;
	options.always_print_primitive_fields = true;
	options.always_print_enums_as_ints = false;
	options.preserve_proto_field_names = true;
	google::protobuf::util::MessageToJsonString(factoryOut, &s, options);

	std::ofstream outFile(outfilename);
	if (outFile.is_open())
	{
		outFile << s;
		outFile.close();
	}
	else
	{
		std::cout << "Could not open file " << outfilename << std::endl;
	}
}

} /* namespace mabz */