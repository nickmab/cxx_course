#ifndef ADV_CXX_INHERIT_H
#define ADV_CXX_INHERIT_H

namespace mabz {

class Parent
{
private:
	int one; // private, not protected, so child classes can't touch it.

public:
	// Implementing _another_ constructor means you lose the _default_
	// constructor, and so need to re-provide it. 
	// Child classes will automatically try to call the corresponding
	// parent constructors - so need to make sure there's the right overlap.
	Parent();
	Parent(const Parent&);
	
	// virtual ~Parent() {}

	int One() const { return one; }

	// virtual keyword is important in parent - otherwise vtable won't be created.
	virtual void print();
};

class Child: public Parent
{
private:
	int two;

public:
	// Child constructor needs to initialise its own
	// member variables, but this will also require
	// a default constructor on the parent too (which
	// will initialise its own members etc.)
	Child();

	int Two() const { return two; }

	// do not need virtual keyword here.
	void print();
};

} /* namespace mabz */

#endif /* ADV_CXX_LIB_H */