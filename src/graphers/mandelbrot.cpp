#include <cmath>

#include "advanced_cxx/colors/color_scheme.h"
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

void MandelbrotCalc::Run(const BmpGrapher::RunArgs* runArgs)
{
	// currently only one color scheme supported...
	auto args = dynamic_cast<const MandelbrotCalc::RunSingleColorSchemeArgs*>(runArgs);
	if (args == nullptr)
	{
		throw "Invalid args passed to MandelbrotCalc::Run";
	}

	if (!mIterationsCalculated)
	{
		std::cout << "Generating..." << std::endl;
		CacheAllPixelIterations();
	}

	std::cout << "Colorizing..." << std::endl;
	Colorize<mabz::color::SingleColorScheme>(
		mMaxIterations, 
		args->mHundredPerCentRed, args->mHundredPerCentGreen, args->mHundredPerCentBlue,
		args->mScalingBaseRed, args->mScalingBaseGreen, args->mScalingBaseBlue);

	std::cout << "Writing..." << std::endl;
	WriteToFile(args->mOutFilename.c_str());
}

} /* namespace graphers */
} /* namespace mabz */