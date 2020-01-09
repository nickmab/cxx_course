#pragma once

#include <vector>

#include <advanced_cxx/bmp_grapher.h>
#include <advanced_cxx/graphers/color_utils.h>

namespace mabz { namespace graphers { namespace quadratic {

namespace color = mabz::graphers::color;

class QuadraticColorizer
{
public:
	virtual ~QuadraticColorizer() {}
	virtual color::RGB GetColor(double) const = 0;
};

class QuadraticCalc : public BmpGrapher
{
private:
	inline static double EPSILON{0.0000001};

	struct QuadraticRoots
	{
		enum Type { UNDEFINED, ONE, TWO };
		Type mType{Type::UNDEFINED};
		double mLeftXValue{0};
		double mRightXValue{0};
		
		QuadraticRoots() = default;
		QuadraticRoots(QuadraticRoots&&) = default;
	};

	struct Point
	{
		double mX;
		double mY;

		Point(double x, double y) : mX(x), mY(y) {}
		Point(Point&&) = default;
	};

	static double DistanceBetweenTwoPoints(const double x1, const double y1, const double x2, const double y2);

	// representing a quadratic of the form y = ax^2 + bx + c
	static QuadraticRoots SolveQuadratic(const double a, const double b, const double c);
	
	// y = gradient * x + intercept, y = ax^2 + bx + c, returning 0, 1 or 2 points accordingly.
	static std::vector<Point> GetIntersectingPointsBetweenLineAndQuadratic(
		const double gradient, const double intercept, const double a, const double b, const double c);

	// approx. minimum distance between point (x, y) and quadratic y = ax^2 + bx + c.
	static double ApproximateMinDistanceFromPointToQuadratic(
		const double x, const double y, const double a, const double b, const double c);


	// representing a quadratic of the form y = ax^2 + bx + c
	const double mA{0};
	const double mB{0};
	const double mC{0};

	std::vector<double> mDistances;
	bool mDistancesCalculated{false};

	double CalcPixelDistance(const double x, const double y) const;
	void CacheAllPixelDistances();
	void Colorize(const QuadraticColorizer&);

public:
	QuadraticCalc(double xCenter, 
		double yCenter, 
		double xDomainWidth, 
		int pixelWidth,
		int pixelHeight,
		// representing a quadratic of the form y = ax^2 + bx + c
		double a, double b, double c);

	virtual void Run(std::shared_ptr<const BmpGrapher::RunArgs>) override;
};

enum ColorScheme
{
	DUAL
};

struct RunArgs : public BmpGrapher::RunArgs
{
	std::string mOutFilename{""};
	ColorScheme mColorScheme{ColorScheme::DUAL};
	std::shared_ptr<const BmpGrapher::RunArgs> mColorSchemeArgs{nullptr};
	
	RunArgs(std::string a, ColorScheme b, std::shared_ptr<const BmpGrapher::RunArgs> c)
		: mOutFilename(a), mColorScheme(b), mColorSchemeArgs(c)
	{}
	RunArgs(RunArgs&&) = default;
};

class DualColorScheme : public QuadraticColorizer
{
private:
	double mMaxDistance{0};
	color::RGB mBackgroundColor;
	color::HSV mCurveColor;

public:
	
	struct ConstructorArgs;

	DualColorScheme(const ConstructorArgs&);
	
	virtual color::RGB GetColor(double distance) const override;
};

#define ui8 std::uint8_t
struct DualColorScheme::ConstructorArgs : public BmpGrapher::RunArgs
{
	double mMaxDistance{0};
	ui8 mBackgroundRed{0};
	ui8 mBackgroundGreen{0};
	ui8 mBackgroundBlue{0};
	ui8 mCurveRed{0};
	ui8 mCurveGreen{0};
	ui8 mCurveBlue{0};

	ConstructorArgs(double maxDistance, ui8 a, ui8 b, ui8 c, ui8 d, ui8 e, ui8 f)
		: mMaxDistance(maxDistance)
		, mBackgroundRed(a), mBackgroundGreen(b), mBackgroundBlue(c)
		, mCurveRed(d), mCurveGreen(e), mCurveBlue(f)
	{}
};
#undef ui8

} /* namespace quadratic */
} /* namespace graphers */
} /* namespace mabz */
