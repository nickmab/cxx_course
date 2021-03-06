#include <cmath>

#include "advanced_cxx/graphers/mandelbrot.h"

namespace mabz { namespace graphers { namespace mandelbrot {

namespace color = mabz::graphers::color;

void MandelbrotCalc::add(const Cmplx& left, const Cmplx& right, Cmplx& out)
{
	out.Re = left.Re + right.Re;
	out.Im = left.Im + right.Im;
}

void MandelbrotCalc::square(const Cmplx& in, Cmplx& out)
{
	// introduce tmp variables in case in and out are the same object.
	const double outRe = in.Re*in.Re - in.Im*in.Im;
	const double outIm = 2*in.Re*in.Im;
	out.Re = outRe;
	out.Im = outIm;
}

double MandelbrotCalc::abs(const Cmplx& z)
{
	return sqrt(z.Re*z.Re + z.Im*z.Im);
}

MandelbrotCalc::MandelbrotCalc(
	double xCenter, 
	double yCenter, 
	double xDomainWidth, 
	int pixelWidth,
	int pixelHeight,
	int maxIterations)

	: BmpGrapher(xCenter, yCenter, xDomainWidth, pixelWidth, pixelHeight)
	, mMaxIterations(maxIterations)
	, mIterations(pixelWidth * pixelHeight, 0)
{}

int MandelbrotCalc::CalcPixelIterations(const double x, const double y) const
{
	Cmplx z;
	const Cmplx c{x, y};

	int result = 0;
	while (result < mMaxIterations)
	{
		if (MandelbrotCalc::abs(z) > 2.0)
		{
			break;
		}

		// z = z^2 + c;
		MandelbrotCalc::square(z, z);
		MandelbrotCalc::add(z, c, z);
		result++;
	}
	return result;
}

void MandelbrotCalc::CacheAllPixelIterations()
{
	const int width = mBmp.Width();
	const int height = mBmp.Height();
	
	std::cout << "Generating mandelbrot iterations / pixel color scores for " 
			  << width * height << " pixels." << std::endl;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			double scaledX{0};
			double scaledY{0};
			mPixelXYMapper.Convert(x, y, scaledX, scaledY);
			mIterations[y*mBmp.Width() + x] = CalcPixelIterations(scaledX, scaledY);
		}
	}

	mIterationsCalculated = true;
}

void MandelbrotCalc::Colorize(const MandelbrotColorizer& colorizer)
{
	const int width = mBmp.Width();
	const int height = mBmp.Height();
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			const color::RGB rgb = colorizer.GetColor(mIterations[y*width + x]);
			mBmp.SetRGBPixel(x, y, rgb.mRed, rgb.mGreen, rgb.mBlue);
		}
	}
}

void MandelbrotCalc::Run(std::shared_ptr<const BmpGrapher::RunArgs> runArgs)
{
	if (!mIterationsCalculated)
	{
		CacheAllPixelIterations();
	}

	// currently only one color scheme supported...
	namespace nsmandel = mabz::graphers::mandelbrot;
	auto args = dynamic_cast<const nsmandel::RunArgs*>(runArgs.get());
	if (args == nullptr)
	{
		throw "Invalid args passed to MandelbrotCalc::Run";
	}

	if (args->mColorScheme == ColorScheme::SINGLE)
	{
		auto colorSchemeArgs = dynamic_cast<const SingleColorScheme::ConstructorArgs*>(
			args->mColorSchemeArgs.get());

		if (colorSchemeArgs == nullptr)
		{
			throw "Invalid SingleColorScheme::ConstructorArgs, could not cast.";
		}

		SingleColorScheme colorizer(mMaxIterations, *colorSchemeArgs);
		std::cout << "Colorizing (or re-colorizing) Mandelbrot fractal..." << std::endl;
		Colorize(colorizer);
	}
	else
	{
		throw "Invalid mandelbrot color scheme chosen - this must be impossible";	
	}
	
	std::cout << "Writing Mandelbrot fractal to file: " << args->mOutFilename << std::endl;
	WriteToFile(args->mOutFilename.c_str());
}

SingleColorScheme::SingleColorScheme(int maxIterations, const ConstructorArgs& args)
	: mMaxIterations(maxIterations)
	, mMandelbrotColor(args.mMandelbrotRed, args.mMandelbrotGreen, args.mMandelbrotBlue)
	, mBaseColor(std::move(color::RGB::ToHSV(args.mBaseRed, args.mBaseGreen, args.mBaseBlue)))
{}

color::RGB SingleColorScheme::GetColor(int iterations) const
{
	if (iterations == mMaxIterations)
	{
		// makes / returns a copy obviously.
		return mMandelbrotColor;
	}
	else
	{
		const double intensity = sqrt(static_cast<double>(iterations) / mMaxIterations);
		color::HSV hsv(mBaseColor);
		hsv.mHue *= intensity;
		hsv.mSaturation *= intensity;
		hsv.mValue *= intensity;
		return std::move(hsv.ToRGB());
	}
}

} /* namespace mandelbrot */
} /* namespace graphers */
} /* namespace mabz */