#include <cmath>

#include "advanced_cxx/graphers/mandelbrot.h"

namespace mabz { namespace graphers {

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
{
	std::cout << "Constructed..." << std::endl;
}

void MandelbrotCalc::CalcPixelIterations(int& outResult, const double x, const double y)
{
	Cmplx z;
	const Cmplx c{x, y};

	outResult = 0;
	while (outResult < mMaxIterations)
	{
		if (MandelbrotCalc::abs(z) > 2.0)
		{
			return;
		}

		// z = z^2 + c;
		MandelbrotCalc::square(z, z);
		MandelbrotCalc::add(z, c, z);
		outResult++;
	}	
}

void MandelbrotCalc::CacheAllPixelIterations()
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
			int result;
			CalcPixelIterations(result, scaledX, scaledY);
			mIterations[y*mBmp.Width() + x] = result;
		}
	}

	mIterationsCalculated = true;
}

void MandelbrotCalc::Run(std::shared_ptr<const BmpGrapher::RunArgs> runArgs)
{
	if (!mIterationsCalculated)
	{
		std::cout << "Generating..." << std::endl;
		CacheAllPixelIterations();
	}

	// currently only one color scheme supported...
	auto args = dynamic_cast<const MandelbrotCalc::RunArgs*>(runArgs.get());
	if (args == nullptr)
	{
		throw "Invalid args passed to MandelbrotCalc::Run";
	}

	if (args->mColorScheme == MandelbrotColorScheme::SINGLE)
	{
		auto colorSchemeArgs = dynamic_cast<const MandelbrotCalc::SingleColorScheme::ConstructorArgs*>(
			args->mColorSchemeArgs.get());

		if (colorSchemeArgs == nullptr)
		{
			throw "Invalid MandelbrotCalc::SingleColorScheme::ConstructorArgs, could not cast.";
		}

		std::cout << "Colorizing..." << std::endl;
		Colorize<MandelbrotCalc::SingleColorScheme>(mMaxIterations, *colorSchemeArgs);
	}
	else
	{
		throw "Invalid mandelbrot color scheme chosen - this must be impossible";	
	}
	
	std::cout << "Writing..." << std::endl;
	WriteToFile(args->mOutFilename.c_str());
}

MandelbrotCalc::SingleColorScheme::SingleColorScheme(int maxIterations, const ConstructorArgs& args)
	: mMaxIterations(maxIterations)
	, mMandelbrotColor(args.mMandelbrotRed, args.mMandelbrotGreen, args.mMandelbrotBlue)
	, mBaseColor(std::move(mabz::color::RGB::ToHSV(args.mBaseRed, args.mBaseGreen, args.mBaseBlue)))
{}

mabz::color::RGB MandelbrotCalc::SingleColorScheme::GetColor(int iterations) const
{
	if (iterations == mMaxIterations)
	{
		// makes / returns a copy obviously.
		return mMandelbrotColor;
	}
	else
	{
		const double intensity = sqrt(static_cast<double>(iterations) / mMaxIterations);
		mabz::color::HSV hsv(mBaseColor);
		hsv.mHue *= intensity;
		hsv.mSaturation *= intensity;
		hsv.mValue *= intensity;
		return std::move(hsv.ToRGB());
	}
}

} /* namespace graphers */
} /* namespace mabz */