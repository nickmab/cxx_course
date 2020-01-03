#ifndef ADVANCED_CXX_MANDELBROT_H
#define ADVANCED_CXX_MANDELBROT_H

namespace mabz { namespace mandelbrot {

class calculator
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
	static const unsigned MAX_ITERATIONS{1500};

	// Tests a complex number by running up to calculator::MAX_ITERATIONS of the mandelbrot fractal
	// calculation to see whether the result diverges / magnitude exceeds 2. 
	// "out" variable indicates the number of iterations done until it crossed through the 2 barrier.
	// Returning "false" indicates that divergence did not appear to occur within the max iterations.
	static bool IsDivergent(unsigned& outIterations, double real, double imaginary);
};

} /* namespace mandelbrot */
} /* namespace mabz */

#endif /* ADVANCED_CXX_MANDELBROT_H */