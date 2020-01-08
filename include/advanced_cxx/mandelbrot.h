#pragma once

#include <advanced_cxx/fractal.h>

namespace mabz { namespace mandelbrot {

class MandelbrotCalc : public PixelScoreCalculator
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

public:
	static const int MAX_ITERATIONS{750};

	// Tests a complex number by running up to calculator::MAX_ITERATIONS of the mandelbrot fractal
	// calculation to see whether the result diverges / magnitude exceeds 2. 
	// "out" variable indicates the number of iterations done until it crossed through the 2 barrier.
	virtual void GetPixelScore(int& outResult, double x, double y) const override;
};

} /* namespace mandelbrot */
} /* namespace mabz */
