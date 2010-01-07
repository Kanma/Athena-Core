/**	@file	StringUtils.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Utils::StringUtils'

    @note   This is based on the corresponding class from
            <a href="http://www.ogre3d.org/">Ogre3D</a>.
*/

#include <Athena-Core/StringUtils.h>

using namespace Athena::Utils;
using namespace std;


/************************************* STATIC METHODS ***********************************/

void StringUtils::replaceAll(string &strSource, const string& strWhat, const string& strWith)
{
	size_t index = strSource.find(strWhat);
	
	while (index != string::npos)
	{
		strSource.erase(index, strWhat.length());
		strSource.insert(index, strWith);
		
		index = strSource.find(strWhat, index + strWith.length());
	}
}

//-----------------------------------------------------------------------

StringUtils::tStringsList StringUtils::split(const string& str, const string& delims)
{
    tStringsList ret;
    
    // Pre-allocate some space for performance
    ret.reserve(10);    // 10 is guessed capacity for most case

    // Use STL methods 
    size_t start, pos;
    start = 0;
    do 
    {
        pos = str.find_first_of(delims, start);
        if (pos == start)
        {
            // Do nothing
            ret.push_back("");
            start = pos + 1;
        }
        else if (pos == string::npos)
        {
            // Copy the rest of the string
            ret.push_back(str.substr(start));
            break;
        }
        else
        {
            // Copy up to delimiter
            ret.push_back(str.substr(start, pos - start));
            start = pos + 1;
        }

    } while ((start != string::npos) && (pos != string::npos));

	return ret;
}
