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

    bool operator ! () const;
    operator bool();
    bool operator == (const Cmplx&) const;
    bool operator != (const Cmplx&) const;
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

    // // "ascending sort" would be from left-to-right, top-to-bottom.
    // // i.e. (-7 - 2i) considered L.T. (-7 + 2i), which is L.T. -6 (+0i) etc.
    CMPLX_OP_COMBOS(>, bool)
    CMPLX_OP_COMBOS(<, bool)
    CMPLX_OP_COMBOS(>=, bool)
    CMPLX_OP_COMBOS(<=, bool)

#undef CMPLX_OP_COMBOS

} // namespace mabz

#endif // CMPLX_NUM_H