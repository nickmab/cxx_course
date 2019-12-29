#ifndef ADV_CXX_TMPLTZ_H
#define ADV_CXX_TMPLTZ_H

#include <iostream>

namespace mabz {

template <typename T>
class Flog
{
private:
	T _obj;

public:
	Flog() {}
	Flog(T obj) : _obj(obj) {}

	template <typename T> // necessary to decl as tmpl otherwise compiler won't see.
	friend std::ostream& operator << (std::ostream&, const Flog<T>&);
};

template <typename T>
std::ostream& operator << (std::ostream& os, const Flog<T>& f)
{
	os << f._obj;
	return os;
}

template <typename T>
void print(const T& obj)
{
	std::cout << obj << std::endl;
}

void fptrdemo();

} /* namespace mabz */

#endif /* ADV_CXX_TMPLTZ_H */