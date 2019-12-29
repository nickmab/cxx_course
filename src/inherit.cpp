#include <iostream>

#include "advanced_cxx/inherit.h"

namespace mabz {

Parent::Parent() 
	: one(1)
{
	std::cout << "Parent dflt constructor" << std::endl;	
}

Parent::Parent(const Parent& other)
	: one(other.One()) 
{
	std::cout << "Parent copy constructor" << std::endl;
}

// virtual keyword necessary on header decl but not impl.
void Parent::print()
{
	std::cout << "Parent printing" << std::endl;
}

Child::Child() 
	: two(2)
{
	std::cout << "Child dflt constructor" << std::endl;	
}

void Child::print()
{
	std::cout << "Child printing" << std::endl;
}

} /* namespace mabz */