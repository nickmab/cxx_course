#pragma once

#include <advanced_cxx/bmp_grapher.h>

namespace mabz { namespace graphers {

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
	
	template <typename Colorizer, typename... Args>
	void Colorize(Args... colorizerConstructorArgs)
	{
		std::cout << "I am here!" << std::endl;
		Colorizer c(colorizerConstructorArgs...);
		const int width = mBmp.Width();
		const int height = mBmp.Height();
		for (int x = 0; x < width; ++x)
		{
			for (int y = 0; y < height; ++y)
			{
				const mabz::color::RGB rgb = c.GetColor(mIterations[y*width + x]);
				mBmp.SetRGBPixel(x, y, rgb.mRed, rgb.mGreen, rgb.mBlue);
			}
		}
	}

public:
	MandelbrotCalc(double xCenter, 
		double yCenter, 
		double xDomainWidth, 
		int pixelWidth,
		int pixelHeight,
		int maxIterations);


	#define ui8 std::uint8_t
	struct RunSingleColorSchemeArgs : public BmpGrapher::RunArgs
	{
		std::string mOutFilename;
		ui8 mHundredPerCentRed;
		ui8 mHundredPerCentGreen;
		ui8 mHundredPerCentBlue;
		ui8 mScalingBaseRed;
		ui8 mScalingBaseGreen;
		ui8 mScalingBaseBlue;

		RunSingleColorSchemeArgs::RunSingleColorSchemeArgs(
			std::string OutFilename,
			ui8 HundredPerCentRed,
			ui8 HundredPerCentGreen,
			ui8 HundredPerCentBlue,
			ui8 ScalingBaseRed,
			ui8 ScalingBaseGreen,
			ui8 ScalingBaseBlue)

			: mOutFilename(OutFilename)
			, mHundredPerCentRed(HundredPerCentRed)
			, mHundredPerCentGreen(HundredPerCentGreen)
			, mHundredPerCentBlue(HundredPerCentBlue)
			, mScalingBaseRed(ScalingBaseRed)
			, mScalingBaseGreen(ScalingBaseGreen)
			, mScalingBaseBlue(ScalingBaseBlue)
		{}

	};
	#undef ui8

	virtual void Run(const BmpGrapher::RunArgs*) override;

};

} /* namespace graphers */
} /* namespace mabz */
