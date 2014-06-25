#include "Settings.h"
#include <boost/filesystem/path.hpp>
#include <boost/lambda/lambda.hpp>
#include <numeric>

namespace bfs = boost::filesystem;

std::string
trm::GetDataFolderPath()
{
	auto p = bfs::path("/") / "sdcard" / "TransportMania";

	return p.make_preferred().string();
}

std::string
trm::GetRelativePath(const std::initializer_list<std::string> & il)
{
	bfs::path p(GetDataFolderPath());

	using namespace boost::lambda;

	std::for_each(std::begin(il), std::end(il), p /= _1);

	return p.string();
}