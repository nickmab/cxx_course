#include <cmath>

#include "advanced_cxx/mandelbrot.h"

namespace mabz { namespace mandelbrot {

void calculator::add(const Cmplx& left, const Cmplx& right, Cmplx& out)
{
	out.Re = left.Re + right.Re;
	out.Im = left.Im + right.Im;
}

void calculator::square(const Cmplx& in, Cmplx& out)
{
	// introduce tmp variables in case in and out are the same object.
	const double outRe = in.Re*in.Re - in.Im*in.Im;
	const double outIm = 2*in.Re*in.Im;
	out.Re = outRe;
	out.Im = outIm;
}

double calculator::abs(const Cmplx& z)
{
	return sqrt(z.Re*z.Re + z.Im*z.Im);
}

bool calculator::IsDivergent(unsigned& outIterations, double real, double imaginary)
{
	Cmplx z;
	const Cmplx c{real, imaginary};

	outIterations = 0;
	while (outIterations < calculator::MAX_ITERATIONS)
	{
		if (calculator::abs(z) > 2.0)
		{
			return true;
		}

		// z = z^2 + c;
		calculator::square(z, z);
		calculator::add(z, c, z);
		outIterations++;
	}

	return false;
}

} /* namespace mandelbrot */
} /* namespace mabz */