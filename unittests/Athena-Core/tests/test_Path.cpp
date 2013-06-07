#include <UnitTest++.h>
#include <Athena-Core/Utils/Path.h>

using namespace Athena::Utils;


SUITE(PathTests_IsAbsolute)
{
#if ATHENA_PLATFORM == ATHENA_PLATFORM_WIN32

    TEST(AbsoluteWindowsPath)
    {
        CHECK(Path::isAbsolute("C:\\some_folder\\hello.txt"));
    }

    TEST(AbsoluteWindowsPathWithSlashes)
    {
        CHECK(Path::isAbsolute("C:/some_folder/hello.txt"));
    }

#else

    TEST(AbsoluteUnixPath)
    {
        CHECK(Path::isAbsolute("/home/johndoe/hello.txt"));
    }

#endif

    TEST(RelativeUnixPath)
    {
        CHECK(!Path::isAbsolute("./hello.txt"));
    }

    TEST(RelativeWindowsPath)
    {
        CHECK(!Path::isAbsolute("some_folder\\hello.txt"));
    }
}
