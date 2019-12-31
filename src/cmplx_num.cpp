#include <cmath>
#include <sstream>

#include "advanced_cxx/cmplx_num.h"

namespace mabz {

Cmplx::Cmplx()
    : _r(0), _i(0)
{
    std::cout << "Cmplx dflt constr (" << this << ")" << std::endl;
}

Cmplx::Cmplx(double r, double i)
    : _r(r), _i(i)
{
    std::cout << "Cmplx args constr (" << this << ")" << std::endl;
}

Cmplx::Cmplx(const Cmplx& other)
    : _r(other._r), _i(other._i)
{
    std::cout << "Cmplx copy constr (" << this << ")" << std::endl;
}

Cmplx::Cmplx(const Cmplx&& other) noexcept
    : _r(other._r), _i(other._i)
{
    std::cout << "Cmplx move constr (" << this << ")" << std::endl;
}

Cmplx::~Cmplx()
{
    std::cout << "Cmplx destr (" << this << ")" << std::endl;
}

double Cmplx::Abs() const
{
    return sqrt(_r*_r + _i*_i);
}

Cmplx& Cmplx::operator = (const Cmplx& other)
{
    std::cout << "Cmplx assign op (" << this << ")" << std::endl;
    
    // no need for a self-assignment guard here.
    _r = other._r;
    _i = other._i;
    return *this;
}

// returns complex conjugate --> simply negate the imaginary part.
Cmplx Cmplx::operator * () const
{
    return Cmplx(_r, -_i);
}

bool Cmplx::DoublesEqual(double a, double b)
{
    return abs(a - b) < Cmplx::CMP_EPSILON;   
}

Cmplx::operator bool () const
{
    return !Cmplx::DoublesEqual(_r, 0.) || !Cmplx::DoublesEqual(_i, 0.);
    //return !(!*this);
}

bool Cmplx::operator == (const Cmplx& other) const
{
    const bool rEqual = Cmplx::DoublesEqual(_r, other._r);
    return (!rEqual) ? false : Cmplx::DoublesEqual(_i, other._i);
}

bool Cmplx::operator != (const Cmplx& other) const
{
    return !(*this == other);
}

// complex number multiplication is like expanding a (a+bi)(c+di) polynomial
// where i squared is -1, obviously.
void Cmplx::Calculator::mul(CMPLX_NUM_H_CMPLX_ARGS_SIGNATURE)
{
    outRe = lhsRe*rhsRe - lhsIm*rhsIm;
    outIm = lhsRe*rhsIm + lhsIm*rhsRe;
}

// complex number division is annoying. 
// First multiply both num and denom by the conjugate of the denom.
// This has the effect of removing the imaginary part of the denominator.
// Then simplify by dividing through by the now (real) denominator.
void Cmplx::Calculator::div(CMPLX_NUM_H_CMPLX_ARGS_SIGNATURE)
{
    if (Cmplx::DoublesEqual(rhsRe, 0) && Cmplx::DoublesEqual(rhsIm, 0))
    {
        std::stringstream err;
        err << "Attempted to divide by zero! "
            << "Numerator(" << lhsRe << ", " << lhsIm << ") Denominator(" 
            << rhsRe << ", " << rhsIm << ")";
        throw err.str();
    }

    const double c2plusd2 = rhsRe*rhsRe + rhsIm*rhsIm;
    outRe = (lhsRe*rhsRe + lhsIm*rhsIm) / c2plusd2;
    outIm = (lhsIm*rhsRe + lhsRe*rhsIm) / c2plusd2;
}

std::ostream& operator << (std::ostream& os, const Cmplx& c)
{
    os << "(" << c.Re() << (c.Im() > -Cmplx::CMP_EPSILON ? " + " : " - ")
       << abs(c.Im()) << "i)" << " at " << &c;
    return os;
}

// This is horrible and I don't recommend it; just doing it for practice.
// using macros / preprocessor to generate all the combinations of code
// to implement numeric operator arithmetic and comparison overloading.

#define COMPLEX_ARGS_TO_PARTS \
    double lhsRe; \
    double lhsIm; \
    double rhsRe; \
    double rhsIm; \
    double outRe; \
    double outIm; \
    Cmplx::Calculator::ToParts(lhsRe, lhsIm, lhs); \
    Cmplx::Calculator::ToParts(rhsRe, rhsIm, rhs); 

#define CMPLX_ARITHMETIC_INNER_CODE(_func, _lhs_rval, _rhs_rval) \
    COMPLEX_ARGS_TO_PARTS \
    Cmplx::Calculator::_func(outRe, outIm, lhsRe, lhsIm, rhsRe, rhsIm); \
    return Cmplx(outRe, outIm);

#define CMPLX_ARITHMETIC_MACRO(_op, _func) \
    Cmplx operator _op (const Cmplx& lhs, const Cmplx& rhs) \
    { \
        CMPLX_ARITHMETIC_INNER_CODE(_func,,) \
    } \
\
    Cmplx operator _op (const Cmplx& lhs, double rhs) \
    { \
        CMPLX_ARITHMETIC_INNER_CODE(_func,,) \
    } \
\
    Cmplx operator _op (double lhs, const Cmplx& rhs) \
    { \
        CMPLX_ARITHMETIC_INNER_CODE(_func,,) \
    }

    CMPLX_ARITHMETIC_MACRO(+, add)
    CMPLX_ARITHMETIC_MACRO(-, sub)
    CMPLX_ARITHMETIC_MACRO(*, mul)
    CMPLX_ARITHMETIC_MACRO(/, div)

#undef CMPLX_ARITHMETIC_MACRO
#undef COMPLEX_ARGS_TO_PARTS
#undef CMPLX_ARITHMETIC_INNER_CODE
#undef CMPLX_NUM_H_CMPLX_ARGS_SIGNATURE

// This is terrible code; never ever do this.
#define GT_LT(_op1, _op2) \
    bool operator _op1 (const Cmplx& a, const Cmplx& b) \
    { \
        const bool reIsDecisive = a.Re() _op1 (b.Re() _op2 Cmplx::CMP_EPSILON); \
        return reIsDecisive ? true : a.Im() _op1 (b.Im() _op2 Cmplx::CMP_EPSILON); \
    } \
\
    bool operator _op1 (const Cmplx& a, double b) \
    { \
        const bool reIsDecisive = a.Re() _op1 (b _op2 Cmplx::CMP_EPSILON); \
        return reIsDecisive ? true : a.Im() _op1 _op2 Cmplx::CMP_EPSILON; \
    } \
\
    bool operator _op1 (double a, const Cmplx& b) \
    { \
        const bool reIsDecisive = a _op1 (b.Re() _op2 Cmplx::CMP_EPSILON); \
        return reIsDecisive ? true : 0. _op1 (b.Im() _op2 Cmplx::CMP_EPSILON); \
    } 

    GT_LT(>, +)
    GT_LT(<, -)
    GT_LT(>=, -)
    GT_LT(<=, +)
#undef GT_LT

} // namespace mabz