#include <algorithm>
#include <cmath>
#include <limits>

#include "advanced_cxx/graphers/quadratic.h"

namespace mabz { namespace graphers { namespace quadratic {

namespace color = mabz::graphers::color;

QuadraticCalc::QuadraticCalc(
	double xCenter, 
	double yCenter, 
	double xDomainWidth, 
	int pixelWidth,
	int pixelHeight,
	// representing a quadratic of the form y = ax^2 + bx + c
	double a, double b, double c)

	: BmpGrapher(xCenter, yCenter, xDomainWidth, pixelWidth, pixelHeight)
	, mA(a), mB(b), mC(c)
	, mDistances(pixelWidth * pixelHeight, 0)
{}

double QuadraticCalc::DistanceBetweenTwoPoints(const double x1, const double y1, const double x2, const double y2)
{
	const double xDiff = x1 - x2;
	const double yDiff = y1 - y2;
	return sqrt(xDiff*xDiff + yDiff*yDiff);
}

QuadraticCalc::QuadraticRoots QuadraticCalc::SolveQuadratic(const double a, const double b, const double c)
{
	const double rootedPart = b*b - 4*a*c;
	QuadraticRoots result;
	
	if (rootedPart < 0)
	{
		result.mType = QuadraticRoots::Type::UNDEFINED;
	}
	else if (rootedPart < EPSILON)
	{
		result.mType = QuadraticRoots::Type::ONE;
		result.mLeftXValue = result.mRightXValue = -b / (2*a);
	}
	else
	{
		result.mType = QuadraticRoots::Type::TWO;
		result.mLeftXValue = (-b - sqrt(rootedPart)) / (2*a);
		result.mRightXValue = (-b + sqrt(rootedPart)) / (2*a);
	}

	return std::move(result);
}

std::vector<QuadraticCalc::Point> QuadraticCalc::GetIntersectingPointsBetweenLineAndQuadratic(
	const double gradient, const double intercept, const double a, const double b, const double c)
{
	std::vector<QuadraticCalc::Point> result;

	const QuadraticCalc::QuadraticRoots xValues = QuadraticCalc::SolveQuadratic(
		a, b - gradient, c - intercept);

	if (xValues.mType != QuadraticRoots::Type::UNDEFINED)
	{
		const double yValue = gradient * xValues.mLeftXValue + intercept;
		result.emplace_back(xValues.mLeftXValue, yValue);
	}
	if (xValues.mType == QuadraticRoots::Type::TWO)
	{
		const double yValue = gradient * xValues.mRightXValue + intercept;
		result.emplace_back(xValues.mRightXValue, yValue);
	}

	return std::move(result);
}

double QuadraticCalc::ApproximateMinDistanceFromPointToQuadratic(
	const double x, const double y, const double a, const double b, const double c)
{
	// Super crude algorithm for now; try vertical, horizontal and both 45 degree lines
	// passing through this point in order to see if they cross the quadratic. 
	// Compute the minimum distance found between the point and the quadratic
	// along one of these lines and return that as the result.
	double minimumFound = std::numeric_limits<double>::max();

	// vertical
	{
		const double yValueOfQuadratic = a*x*x + b*x + c;
		minimumFound = std::min(minimumFound, std::fabs(y - yValueOfQuadratic));
	}

	// horizontal line --> where y == y for all x values
	{
		auto points = QuadraticCalc::GetIntersectingPointsBetweenLineAndQuadratic(0, y, a, b, c);
		for (auto& point : points)
		{
			minimumFound = std::min(minimumFound, std::fabs(x - point.mX));
		}
	}

	// 45 degree positive gradient line

	{
		auto points = QuadraticCalc::GetIntersectingPointsBetweenLineAndQuadratic(1.0, y - x, a, b, c);
		for (auto& point : points)
		{
			minimumFound = std::min(minimumFound, 
				QuadraticCalc::DistanceBetweenTwoPoints(x, y, point.mX, point.mY));
		}
	}

	// 45 degree negative gradient line
	{
		auto points = QuadraticCalc::GetIntersectingPointsBetweenLineAndQuadratic(-1.0, y + x, a, b, c);
		for (auto& point : points)
		{
			minimumFound = std::min(minimumFound, 
				QuadraticCalc::DistanceBetweenTwoPoints(x, y, point.mX, point.mY));
		}
	}

	return minimumFound;
}

double QuadraticCalc::CalcPixelDistance(const double x, const double y) const
{
	return QuadraticCalc::ApproximateMinDistanceFromPointToQuadratic(x, y, mA, mB, mC);
}

void QuadraticCalc::CacheAllPixelDistances()
{
	const int width = mBmp.Width();
	const int height = mBmp.Height();
	
	std::cout << "Generating quadratic function / pixel color scores for " 
			  << width * height << " pixels." << std::endl;

	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			double scaledX{0};
			double scaledY{0};
			mPixelXYMapper.Convert(x, y, scaledX, scaledY);
			mDistances[y*mBmp.Width() + x] = CalcPixelDistance(scaledX, scaledY);
		}
	}

	mDistancesCalculated = true;
}

void QuadraticCalc::Colorize(const QuadraticColorizer& colorizer)
{
	const int width = mBmp.Width();
	const int height = mBmp.Height();
	for (int x = 0; x < width; ++x)
	{
		for (int y = 0; y < height; ++y)
		{
			const color::RGB rgb = colorizer.GetColor(mDistances[y*width + x]);
			mBmp.SetRGBPixel(x, y, rgb.mRed, rgb.mGreen, rgb.mBlue);
		}
	}
}

void QuadraticCalc::Run(std::shared_ptr<const BmpGrapher::RunArgs> runArgs)
{
	if (!mDistancesCalculated)
	{
		CacheAllPixelDistances();
	}

	// currently only one color scheme supported...
	namespace nsquadratic = mabz::graphers::quadratic;
	auto args = dynamic_cast<const nsquadratic::RunArgs*>(runArgs.get());
	if (args == nullptr)
	{
		throw "Invalid args passed to QuadraticCalc::Run";
	}

	if (args->mColorScheme == ColorScheme::DUAL)
	{
		auto colorSchemeArgs = dynamic_cast<const DualColorScheme::ConstructorArgs*>(
			args->mColorSchemeArgs.get());

		if (colorSchemeArgs == nullptr)
		{
			throw "Invalid DualColorScheme::ConstructorArgs, could not cast.";
		}

		DualColorScheme colorizer(*colorSchemeArgs);
		std::cout << "Colorizing (or re-colorizing) quadratic graph..." << std::endl;
		Colorize(colorizer);
	}
	else
	{
		throw "Invalid quadratic color scheme chosen - this must be impossible";	
	}
	
	std::cout << "Writing Quadratic image to file: " << args->mOutFilename << std::endl;
	WriteToFile(args->mOutFilename.c_str());
}

DualColorScheme::DualColorScheme(const ConstructorArgs& args)
	: mMaxDistance(args.mMaxDistance)
	, mBackgroundColor(args.mBackgroundRed, args.mBackgroundGreen, args.mBackgroundBlue)
	, mCurveColor(std::move(color::RGB::ToHSV(args.mCurveRed, args.mCurveGreen, args.mCurveBlue)))
{}

color::RGB DualColorScheme::GetColor(double distance) const
{
	if (distance >= mMaxDistance)
	{
		// makes / returns a copy obviously.
		return mBackgroundColor;
	}
	else
	{
		const double intensity = 1 - sqrt(distance / mMaxDistance);
		color::HSV hsv(mCurveColor);
		hsv.mHue *= intensity;
		hsv.mSaturation *= intensity;
		hsv.mValue *= intensity;
		return std::move(hsv.ToRGB());
	}
}

} /* namespace quadratic */
} /* namespace graphers */
} /* namespace mabz */