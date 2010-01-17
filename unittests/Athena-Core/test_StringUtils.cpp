#include <UnitTest++.h>
#include <Athena-Core/Utils/StringUtils.h>

using namespace Athena;
using namespace Athena::Utils;
using namespace std;


SUITE(StringUtilsTests)
{
    TEST(ReplaceAll_OneOccurence_InMiddle)
    {
        string s = "abcdefgh";

        StringUtils::replaceAll(s, "cd", "xy");

    	CHECK_EQUAL("abxyefgh", s);
    }


    TEST(ReplaceAll_OneOccurence_AtBeginning)
    {
        string s = "abcdefgh";

        StringUtils::replaceAll(s, "ab", "xy");

    	CHECK_EQUAL("xycdefgh", s);
    }


    TEST(ReplaceAll_OneOccurence_AtEnd)
    {
        string s = "abcdefgh";

        StringUtils::replaceAll(s, "gh", "xy");

    	CHECK_EQUAL("abcdefxy", s);
    }


    TEST(ReplaceAll_NoOccurence)
    {
        string s = "abcdefgh";

        StringUtils::replaceAll(s, "mn", "xy");

    	CHECK_EQUAL("abcdefgh", s);
    }


    TEST(ReplaceAll_SeveralOccurences)
    {
        string s = "abcdabefabghab";

        StringUtils::replaceAll(s, "ab", "xy");

    	CHECK_EQUAL("xycdxyefxyghxy", s);
    }


    TEST(ReplaceAll_ExactMatch)
    {
        string s = "abcdefgh";

        StringUtils::replaceAll(s, "abcdefgh", "xy");

    	CHECK_EQUAL("xy", s);
    }


    TEST(Split_OneOccurence_InMiddle)
    {
        string s = "abcd;efgh";

        StringUtils::tStringsList result = StringUtils::split(s, ";");

        CHECK_EQUAL(2, result.size());
    	CHECK_EQUAL("abcd", result[0]);
    	CHECK_EQUAL("efgh", result[1]);
    }


    TEST(Split_OneOccurence_AtBeginning)
    {
        string s = ";abcdefgh";

        StringUtils::tStringsList result = StringUtils::split(s, ";");

        CHECK_EQUAL(2, result.size());
    	CHECK_EQUAL("", result[0]);
    	CHECK_EQUAL("abcdefgh", result[1]);
    }


    TEST(Split_OneOccurence_AtEnd)
    {
        string s = "abcdefgh;";

        StringUtils::tStringsList result = StringUtils::split(s, ";");

        CHECK_EQUAL(2, result.size());
    	CHECK_EQUAL("abcdefgh", result[0]);
    	CHECK_EQUAL("", result[1]);
    }


    TEST(Split_NoOccurence)
    {
        string s = "abcdefgh";

        StringUtils::tStringsList result = StringUtils::split(s, ";");

        CHECK_EQUAL(1, result.size());
    	CHECK_EQUAL("abcdefgh", result[0]);
    }


    TEST(Split_SeveralOccurences)
    {
        string s = ";ab;cd;ef;gh;";

        StringUtils::tStringsList result = StringUtils::split(s, ";");

        CHECK_EQUAL(6, result.size());
    	CHECK_EQUAL("", result[0]);
    	CHECK_EQUAL("ab", result[1]);
    	CHECK_EQUAL("cd", result[2]);
    	CHECK_EQUAL("ef", result[3]);
    	CHECK_EQUAL("gh", result[4]);
    	CHECK_EQUAL("", result[5]);
    }
}
