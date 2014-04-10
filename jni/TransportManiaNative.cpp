#include <jni.h>
#include <string>
extern "C" {
const char * test()
{
	static std::string a = "asdfs";
	auto asd = [](){return "sdfhj";};
	a = asd();
	return a.c_str();
}}
