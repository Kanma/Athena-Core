#include <UnitTest++.h>
#include <Athena-Core/Data/FileDataStream.h>

using namespace Athena::Data;
using namespace std;


SUITE(FileDataStreamTests)
{
    TEST(OpenExistingFile)
    {
        FileDataStream stream(ATHENA_CORE_UNITTESTS_DATA_PATH "lines.txt");
        CHECK(stream.isOpen());
        CHECK(!stream.eof());
    }


    TEST(OpenUnknownFile)
    {
        FileDataStream stream("unknown.txt");
        CHECK(!stream.isOpen());
    }


    TEST(GetLine)
    {
        FileDataStream stream(ATHENA_CORE_UNITTESTS_DATA_PATH "lines.txt");

        string s = stream.getLine();
        CHECK_EQUAL("Line 1", s);

        s = stream.getLine();
        CHECK_EQUAL("Line 2", s);

        s = stream.getLine();
        CHECK_EQUAL("Line 3", s);

        s = stream.getLine();
        CHECK_EQUAL("", s);

        s = stream.getLine();
        CHECK_EQUAL("Line 5", s);

        CHECK(stream.eof());
    }


    TEST(ReadLine)
    {
        FileDataStream stream(ATHENA_CORE_UNITTESTS_DATA_PATH "lines.txt");

        char buf[31];

        size_t c = stream.readLine(buf, 30);
        CHECK_EQUAL(6, c);
        CHECK_EQUAL("Line 1", buf);

        c = stream.readLine(buf, 3);
        CHECK_EQUAL(3, c);
        CHECK_EQUAL("Lin", buf);

        c = stream.readLine(buf, 30);

        c = stream.readLine(buf, 30, " ");
        CHECK_EQUAL(4, c);
        CHECK_EQUAL("Line", buf);
    }


    TEST(SkipLine)
    {
        FileDataStream stream(ATHENA_CORE_UNITTESTS_DATA_PATH "lines.txt");

        size_t c = stream.skipLine();
        CHECK_EQUAL(7, c);

        string s = stream.getLine();
        CHECK_EQUAL("Line 2", s);

        c = stream.skipLine(" ");
        CHECK_EQUAL(5, c);

        s = stream.getLine();
        CHECK_EQUAL("3", s);
    }


    TEST(Read)
    {
        FileDataStream stream(ATHENA_CORE_UNITTESTS_DATA_PATH "lines.txt");

        char buf[31];

        size_t c = stream.read(buf, 10);
        buf[c] = '\0';
        CHECK_EQUAL(10, c);
        CHECK_EQUAL("Line 1\nLin", buf);

        c = stream.read(buf, 10);
        buf[c] = '\0';
        CHECK_EQUAL(10, c);
        CHECK_EQUAL("e 2\nLine 3", buf);

        c = stream.read(buf, 30);
        buf[c] = '\0';
        CHECK_EQUAL(8, c);
        CHECK_EQUAL("\n\nLine 5", buf);

        CHECK(stream.eof());
    }


    TEST(Skip)
    {
        FileDataStream stream(ATHENA_CORE_UNITTESTS_DATA_PATH "lines.txt");

        char buf[31];

        stream.skip(5);

        CHECK_EQUAL(5, stream.tell());

        size_t c = stream.read(buf, 1);
        buf[c] = '\0';
        CHECK_EQUAL("1", buf);

        stream.skip(6);

        CHECK_EQUAL(12, stream.tell());

        c = stream.read(buf, 1);
        buf[c] = '\0';
        CHECK_EQUAL("2", buf);

        stream.skip(-8);

        CHECK_EQUAL(5, stream.tell());

        c = stream.read(buf, 1);
        buf[c] = '\0';
        CHECK_EQUAL("1", buf);
    }


    TEST(Seek)
    {
        FileDataStream stream(ATHENA_CORE_UNITTESTS_DATA_PATH "lines.txt");

        char buf[31];

        stream.seek(5);

        CHECK_EQUAL(5, stream.tell());

        size_t c = stream.read(buf, 1);
        buf[c] = '\0';
        CHECK_EQUAL("1", buf);

        stream.seek(5);

        CHECK_EQUAL(5, stream.tell());

        c = stream.read(buf, 1);
        buf[c] = '\0';
        CHECK_EQUAL("1", buf);
    }
}
