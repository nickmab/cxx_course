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
    : _r(other.Re()), _i(other.Im())
{
    std::cout << "Cmplx copy constr (" << this << ")" << std::endl;
}

Cmplx::Cmplx(const Cmplx&& other) noexcept
    : _r(other.Re()), _i(other.Im())
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
    _r = other.Re();
    _i = other.Im();
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

bool Cmplx::operator ! () const
{
    return Cmplx::DoublesEqual(_r, 0.) && Cmplx::DoublesEqual(_i, 0.);
}

Cmplx::operator bool ()
{
    return !(!*this);
}

bool Cmplx::operator == (const Cmplx& other) const
{
    const bool rEqual = Cmplx::DoublesEqual(_r, other.Re());
    return (!rEqual) ? false : Cmplx::DoublesEqual(_i, other.Im());
}

bool Cmplx::operator != (const Cmplx& other) const
{
    return !(*this == other);
}

std::ostream& operator << (std::ostream& os, const Cmplx& c)
{
    os << "(" << c.Re() << (c.Im() > -Cmplx::CMP_EPSILON ? " + " : " ")
       << c.Im() << "i sdklfjsdlkfjsdilkj)" << " at " << &c;
    return os;
}

// This is horrible and I don't recommend it; just doing it for practice.
#define PLUS_AND_MINUS(_op) \
    Cmplx operator _op (const Cmplx& a, const Cmplx& b) \
    { \
        return Cmplx(a.Re() _op b.Re(), a.Im() _op b.Im()); \
    } \
\
    Cmplx operator _op (const Cmplx& a, double b) \
    { \
        return Cmplx(a.Re() _op b, a.Im()); \
    } \
\
    Cmplx operator _op (double a, const Cmplx& b) \
    { \
        return Cmplx(a _op b.Re(), _op b.Im()); \
    }

    PLUS_AND_MINUS(+)
    PLUS_AND_MINUS(-)

#undef PLUS_AND_MINUS

// complex number multiplication is like expanding a (a+bi)(c+di) polynomial
// where i squared is -1, obviously.
Cmplx operator * (const Cmplx& first, const Cmplx& second)
{
    const double a = first.Re();
    const double b = first.Im();
    const double c = second.Re();
    const double d = second.Im();
    return Cmplx(a*c - b*d, a*d + b*c);
}

Cmplx operator * (const Cmplx& first, double second)
{
    return Cmplx(second * first.Re(), second * first.Im());
}

Cmplx operator * (double first, const Cmplx& second)
{
    return Cmplx(first * second.Re(), first * second.Im());
}

// complex number division is annoying. 
// First multiply both num and denom by the conjugate of the denom.
// This has the effect of removing the imaginary part of the denominator.
// Then simplify by dividing through by the now (real) denominator.
Cmplx operator / (const Cmplx& first, const Cmplx& second)
{
    if (!second)
    {
        std::stringstream err;
        err << "Attempted to divide by zero! "
            << "Numerator(" << first << ") Denominator(" << second << ")";
        throw err.str();
    }
    const Cmplx denomConj = *second;
    const Cmplx newNumerator = denomConj * first;
    const double newDenominator = (denomConj * second).Re();
    // could probably avoid creating an intermediate object with inplace ops here.
    return Cmplx(newNumerator.Re() / newDenominator, newNumerator.Im() / newDenominator);
}

Cmplx operator / (const Cmplx& first, double second)
{
    if (Cmplx::DoublesEqual(second, 0.))
    {
        std::stringstream err;
        err << "Attempted to divide by zero! "
            << "Numerator(" << first << ") Denominator(" << second << ")";
        throw err.str();
    }
    return Cmplx(first.Re() / second, first.Im() / second);
}

Cmplx operator / (double first, const Cmplx& second)
{
    if (!second)
    {
        std::stringstream err;
        err << "Attempted to divide by zero! "
            << "Numerator(" << first << ") Denominator(" << second << ")";
        throw err.str();
    }
    const Cmplx denomConj = *second;
    const Cmplx newNumerator = denomConj * first;
    const double newDenominator = (denomConj * second).Re();
    // could probably avoid creating an intermediate object with inplace ops here.
    return Cmplx(newNumerator.Re() / newDenominator, newNumerator.Im() / newDenominator);
}

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