#include <iostream>
#include <memory>

#include "advanced_cxx/inherit.h"

int main()
{
	std::cout << "make a child..." << std::endl;
	mabz::Child c;
	std::cout << "take child ref as parent..." << std::endl;
	mabz::Parent& p = c;
	std::cout << "make a parent..." << std::endl;
	mabz::Parent p2;

	std::cout << "print child..." << std::endl;
	c.print();
	std::cout << "print child ref'd via parent..." << std::endl;
	p.print();
	std::cout << "print parent..." << std::endl;
	p2.print();

	// This will call parent dflt construct, child dflt
	// construct, and then the parent copy construct.
	mabz::Parent p3 = mabz::Child();
	// Be aware of object slicing when you upcast 
	// ...the parts of Child that don't exist in Parent
	// will be thrown away here...
	
	// this won't work...
	mabz::Child* cp = dynamic_cast<mabz::Child*>(&p3);
	if (cp == nullptr)
	{
		std::cout << "Null, as anticipated." << std::endl;
	}

	// this should still work...
	mabz::Child* cp2 = dynamic_cast<mabz::Child*>(&p);
	if (cp2 != nullptr)
	{
		std::cout << "Not null, as expected: " << cp2->Two() << std::endl;
	}

	return 0;
}