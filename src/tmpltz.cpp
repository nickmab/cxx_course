#include "advanced_cxx/tmpltz.h"

namespace mabz {

void fptrdemo()
{
	Flog<int> f(8);
	void (*pf)(const Flog<int>&) = print;
	pf(f);
}

} /* namespace mabz */