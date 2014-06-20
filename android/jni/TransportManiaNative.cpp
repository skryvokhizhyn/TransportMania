#include <jni.h>
#include <string>
#include <stdexcept>

struct Base
{
	virtual ~Base() {}
};

struct Derived : Base
{
};

extern "C" {
const char * test()
{
	try
	{
		throw std::runtime_error("exc");
	}
	catch (const std::exception & e)
	{
		(e);
	}
	
	Base * b = new Derived();
	Derived * d = dynamic_cast<Derived*>(b);
	delete d;
	
	static std::string a = "asdfs";
	auto asd = [](){return "sdfhj";};
	a = asd();
	return a.c_str();
}}
