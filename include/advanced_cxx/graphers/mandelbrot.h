#pragma once

#include <advanced_cxx/bmp_grapher.h>
#include <advanced_cxx/graphers/color_utils.h>

namespace mabz { namespace graphers { namespace mandelbrot {

namespace color = mabz::graphers::color;

class MandelbrotColorizer
{
public:
	virtual ~MandelbrotColorizer() {}
	virtual color::RGB GetColor(int) const = 0;
};

class MandelbrotCalc : public BmpGrapher
{
private:
	struct Cmplx {
		double Re{0};
		double Im{0};
	};
	// complex number arithmetic - just the minimal set required to efficiently do this fractal calc.
	static void add(const Cmplx& left, const Cmplx& right, Cmplx& out);
	static void square(const Cmplx& in, Cmplx& out);
	static double abs(const Cmplx&);

	const int mMaxIterations{0};

	std::vector<int> mIterations;
	bool mIterationsCalculated{false};

	// Tests a complex number by running up to calculator::MAX_ITERATIONS of the mandelbrot fractal
	// calculation to see whether the result diverges / magnitude exceeds 2. 
	// "out" variable indicates the number of iterations done until it crossed through the 2 barrier.
	void CalcPixelIterations(int& outResult, const double x, const double y);
	void CacheAllPixelIterations();
	void Colorize(const MandelbrotColorizer&);

public:
	MandelbrotCalc(double xCenter, 
		double yCenter, 
		double xDomainWidth, 
		int pixelWidth,
		int pixelHeight,
		int maxIterations);

	virtual void Run(std::shared_ptr<const BmpGrapher::RunArgs>) override;
};

enum ColorScheme
{
	SINGLE
};

struct RunArgs : public BmpGrapher::RunArgs
{
	std::string mOutFilename{""};
	ColorScheme mColorScheme{ColorScheme::SINGLE};
	std::shared_ptr<const BmpGrapher::RunArgs> mColorSchemeArgs{nullptr};
	
	RunArgs(std::string a, ColorScheme b, std::shared_ptr<const BmpGrapher::RunArgs> c)
		: mOutFilename(a), mColorScheme(b), mColorSchemeArgs(c)
	{}
	RunArgs(RunArgs&&) = default;
};

class SingleColorScheme : public MandelbrotColorizer
{
private:
	int mMaxIterations;
	color::RGB mMandelbrotColor;
	color::HSV mBaseColor;

public:
	
	struct ConstructorArgs;

	// mScalingDenominator, RGB of mMandelbrotColor, RGB of mBaseColor.
	SingleColorScheme(int maxIterations, const ConstructorArgs&);
	
	virtual color::RGB GetColor(int iterations) const override;
};

#define ui8 std::uint8_t
struct SingleColorScheme::ConstructorArgs : public BmpGrapher::RunArgs
{
	ui8 mMandelbrotRed;
	ui8 mMandelbrotGreen;
	ui8 mMandelbrotBlue;
	ui8 mBaseRed;
	ui8 mBaseGreen;
	ui8 mBaseBlue;

	ConstructorArgs(ui8 a, ui8 b, ui8 c, ui8 d, ui8 e, ui8 f)
		: mMandelbrotRed(a), mMandelbrotGreen(b), mMandelbrotBlue(c), mBaseRed(d), mBaseGreen(e), mBaseBlue(f)
	{}
};
#undef ui8

} /* namespace mandelbrot */
} /* namespace graphers */
} /* namespace mabz */
