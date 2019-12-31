#ifndef CMPLX_NUM_H
#define CMPLX_NUM_H

#include <iostream>

namespace mabz {

class Cmplx
{
private:
    double _r;
    double _i;

public:
    // constructors and destructors...
    Cmplx();
    Cmplx(double, double);
    Cmplx(const Cmplx&);
    Cmplx(const Cmplx&&) noexcept;
    ~Cmplx();

    // accessors
    double Re() const { return _r; }
    double Im() const { return _i; }

    // other properties
    double Abs() const; // the "magnitude" or "absolue value".

    // operator overloads
    Cmplx& operator = (const Cmplx&);
    Cmplx operator * () const; // dereference yields the complex conjugate
    
    static constexpr double CMP_EPSILON = 1e-10;
    static bool DoublesEqual(double, double);

    explicit operator bool() const;
    bool operator == (const Cmplx&) const;
    bool operator != (const Cmplx&) const;

    // unary minus operator
    Cmplx operator - () const { return Cmplx(-_r, -_i); }
    
    // for use with arithmetic operations / operator overloads.
    class Calculator;
};

// Private class used for calculating results of arithmetic operations
// from operator overloeds of the Cmplx class. There will be many
// different function signatures for the Cmplx class itself (below)
class Cmplx::Calculator
{
// note that these macros get undef'd in the cpp file.
#define CMPLX_NUM_H_CMPLX_ARGS_SIGNATURE \
    double& outRe, double& outIm, \
    double lhsRe, double lhsIm, \
    double rhsRe, double rhsIm 

#define CMPLX_NUM_H_CMPLX_CMP_SIGNATURE \
    Cmplx::Calculator::CMP_OP op, double lhsRe, double lhsIm, \
    double rhsRe, double rhsIm

public:
    // getting the real and imaginary parts...
    static void ToParts(double& outRe, double& outIm, double realScalar)
        { outRe = realScalar; outIm = 0; }

    static void ToParts(double& outRe, double& outIm, const Cmplx& c)
        { outRe = c.Re(); outIm = c.Im(); }

    // arithmetic calculations
    static void add(CMPLX_NUM_H_CMPLX_ARGS_SIGNATURE) 
        { outRe = lhsRe + rhsRe; outIm = lhsIm + rhsIm; }
    
    static void sub(CMPLX_NUM_H_CMPLX_ARGS_SIGNATURE) 
        { outRe = lhsRe - rhsRe; outIm = lhsIm - rhsIm; }
    
    // will not inline as these two are more complex (geddit?)
    static void mul(CMPLX_NUM_H_CMPLX_ARGS_SIGNATURE);
    static void div(CMPLX_NUM_H_CMPLX_ARGS_SIGNATURE);

    // comparison operators
    // "ascending sort" would be from left-to-right, top-to-bottom.
    // i.e. (-7 - 2i) considered L.T. (-7 + 2i), which is L.T. -6 (+0i) etc.
    enum CMP_OP { GT, LT, GE, LE };
    static bool cmp(CMPLX_NUM_H_CMPLX_CMP_SIGNATURE);
};

// more operator overloads...
std::ostream& operator << (std::ostream&, const Cmplx&);

#define CMPLX_OP_COMBOS(_op, _T) \
    _T operator _op (const Cmplx&, const Cmplx&); \
    _T operator _op (const Cmplx&, double); \
    _T operator _op (double, const Cmplx&); 

    CMPLX_OP_COMBOS(+, Cmplx)
    CMPLX_OP_COMBOS(-, Cmplx)
    CMPLX_OP_COMBOS(*, Cmplx)
    CMPLX_OP_COMBOS(/, Cmplx)
    
    CMPLX_OP_COMBOS(>, bool)
    CMPLX_OP_COMBOS(<, bool)
    CMPLX_OP_COMBOS(>=, bool)
    CMPLX_OP_COMBOS(<=, bool)

#undef CMPLX_OP_COMBOS

} // namespace mabz

#endif // CMPLX_NUM_H