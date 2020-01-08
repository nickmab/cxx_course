#include <cmath>

#include "advanced_cxx/calculators/mandelbrot.h"

namespace mabz { namespace mandelbrot {

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

void MandelbrotCalc::GetPixelScore(int& outResult, double x, double y) const
{
	Cmplx z;
	const Cmplx c{x, y};

	outResult = 0;
	while (outResult < MandelbrotCalc::MAX_ITERATIONS)
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

} /* namespace mandelbrot */
} /* namespace mabz */