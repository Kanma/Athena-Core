/**	@file	StringConverter.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Utils::StringConverter'

    @note   This is based on the corresponding class from
            <a href="http://www.ogre3d.org/">Ogre3D</a>.
*/

#include <Athena-Core/Utils/StringConverter.h>
#include <Athena-Core/Utils/StringUtils.h>
#include <sstream>

using namespace Athena::Utils;
using namespace Athena::Math;
using namespace std;


/************************************* STATIC METHODS ***********************************/

string StringConverter::toString(Real val, unsigned short precision, unsigned short width,
                                 char fill, std::ios::fmtflags flags)
{
    stringstream stream;
    stream.precision(precision);
    stream.width(width);
    stream.fill(fill);
    if (flags)
        stream.setf(flags);
    stream << val;
    return stream.str();
}

//-----------------------------------------------------------------------

string StringConverter::toString(int val, unsigned short width, char fill,
                                 std::ios::fmtflags flags)
{
    stringstream stream;
	stream.width(width);
    stream.fill(fill);
    if (flags)
        stream.setf(flags);
    stream << val;
    return stream.str();
}

//-----------------------------------------------------------------------

string StringConverter::toString(unsigned int val, unsigned short width, char fill,
                                 std::ios::fmtflags flags)
{
    stringstream stream;
    stream.width(width);
    stream.fill(fill);
    if (flags)
        stream.setf(flags);
    stream << val;
    return stream.str();
}

//-----------------------------------------------------------------------

string StringConverter::toString(unsigned long val, unsigned short width, char fill,
                                 std::ios::fmtflags flags)
{
    stringstream stream;
    stream.width(width);
    stream.fill(fill);
    if (flags)
        stream.setf(flags);
    stream << val;
    return stream.str();
}

//-----------------------------------------------------------------------

string StringConverter::toString(long val, unsigned short width, char fill,
                                 std::ios::fmtflags flags)
{
    stringstream stream;
	stream.width(width);
    stream.fill(fill);
    if (flags)
        stream.setf(flags);
    stream << val;
    return stream.str();
}

//-----------------------------------------------------------------------

string StringConverter::toString(bool val, bool yesNo)
{
    if (val)
    {
        if (yesNo)
        {
            return "yes";
        }
        else
        {
            return "true";
        }
    }
    else
    {
        if (yesNo)
        {
            return "no";
        }
        else
        {
            return "false";
        }
    }
}

//-----------------------------------------------------------------------

Real StringConverter::parseReal(const std::string& val)
{
	// Use istringstream for direct correspondence with toString
	istringstream str(val);
	Real ret = 0;
	str >> ret;

    return ret;
}

//-----------------------------------------------------------------------
int StringConverter::parseInt(const std::string& val)
{
	// Use istringstream for direct correspondence with toString
	istringstream str(val);
	int ret = 0;
	str >> ret;

    return ret;
}

//-----------------------------------------------------------------------
unsigned int StringConverter::parseUnsignedInt(const std::string& val)
{
	// Use istringstream for direct correspondence with toString
	istringstream str(val);
	unsigned int ret = 0;
	str >> ret;

	return ret;
}

//-----------------------------------------------------------------------

long StringConverter::parseLong(const std::string& val)
{
	// Use istringstream for direct correspondence with toString
	istringstream str(val);
	long ret = 0;
	str >> ret;

	return ret;
}

//-----------------------------------------------------------------------

unsigned long StringConverter::parseUnsignedLong(const std::string& val)
{
	// Use istringstream for direct correspondence with toString
	istringstream str(val);
	unsigned long ret = 0;
	str >> ret;

	return ret;
}

//-----------------------------------------------------------------------

bool StringConverter::parseBool(const std::string& val)
{
	return (StringUtils::startsWith(val, "true") || StringUtils::startsWith(val, "yes") ||
		    StringUtils::startsWith(val, "1"));
}

//-----------------------------------------------------------------------

bool StringConverter::isNumber(const std::string& val)
{
	istringstream str(val);
	float tst;
	str >> tst;
	return !str.fail() && str.eof();
}
