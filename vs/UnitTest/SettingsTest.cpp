#include <boost/test/unit_test.hpp>
#include <boost/filesystem/path.hpp>

#include <Settings.h>

using namespace trm;
namespace bfs = boost::filesystem;

BOOST_AUTO_TEST_CASE(GetRelativePathTest1)
{
	const std::string path = GetRelativePath({"a", "bc", "def"});
	BOOST_CHECK_EQUAL(path, (bfs::path(GetDataFolderPath()) / "a\\bc\\def").string());
}