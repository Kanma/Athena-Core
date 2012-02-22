/** @file   DataStream.cpp
    @author Philip Abbet

    Implementation of the class 'Athena::Data::DataStream'
*/

#include <Athena-Core/Data/DataStream.h>
#include <Athena-Core/Utils/StringUtils.h>
#include <memory.h>
#include <string.h>

using namespace Athena::Data;
using namespace Athena::Utils;
using namespace std;


#define ATHENA_STREAM_TEMP_SIZE 128


/*************************************** METHODS ****************************************/

size_t DataStream::readLine(char* buf, size_t maxCount, const string& delim)
{
    if ((m_mode & READ) == 0)
        return 0;

    // Deal with both Unix & Windows LFs
    bool trimCR = false;
    if (delim.find_first_of('\n') != string::npos)
        trimCR = true;

    char tmpBuf[ATHENA_STREAM_TEMP_SIZE];
    size_t chunkSize = std::min(maxCount, (size_t) ATHENA_STREAM_TEMP_SIZE - 1);
    size_t totalCount = 0;
    size_t readCount;

    while (chunkSize && (readCount = read(tmpBuf, chunkSize)) != 0)
    {
        // Terminate
        tmpBuf[readCount] = '\0';

        // Find first delimiter
        size_t pos = strcspn(tmpBuf, delim.c_str());

        if (pos < readCount)
        {
            // Found terminator, reposition backwards
            skip((long)(pos + 1 - readCount));
        }

        // Are we genuinely copying?
        if (buf)
            memcpy(buf+totalCount, tmpBuf, pos);

        totalCount += pos;

        if (pos < readCount)
        {
            // Trim off trailing CR if this was a CR/LF entry
            if (trimCR && totalCount && buf[totalCount-1] == '\r')
                --totalCount;

            // Found terminator, break out
            break;
        }

        // Adjust chunkSize for next time
        chunkSize = std::min(maxCount - totalCount, (size_t) ATHENA_STREAM_TEMP_SIZE - 1);
    }

    // Terminate
    buf[totalCount] = '\0';

    return totalCount;
}

//-----------------------------------------------------------------------

string DataStream::getLine(bool trimAfter)
{
    // Declarations
    char tmpBuf[ATHENA_STREAM_TEMP_SIZE];
    string retString;
    size_t readCount;

    if ((m_mode & READ) == 0)
        return 0;

    // Keep looping while not hitting delimiter
    while ((readCount = read(tmpBuf, ATHENA_STREAM_TEMP_SIZE-1)) != 0)
    {
        // Terminate string
        tmpBuf[readCount] = '\0';

        char* p = strchr(tmpBuf, '\n');
        if (p)
        {
            // Reposition backwards
            skip((long)(p + 1 - tmpBuf - readCount));
            *p = '\0';
        }

        retString += tmpBuf;

        if (p)
        {
            // Trim off trailing CR if this was a CR/LF entry
            if (retString.length() && retString[retString.length()-1] == '\r')
                retString.erase(retString.length()-1, 1);

            // Found terminator, break out
            break;
        }
    }

    if (trimAfter)
        StringUtils::trim(retString);

    return retString;
}

//-----------------------------------------------------------------------

size_t DataStream::skipLine(const string& delim)
{
    char tmpBuf[ATHENA_STREAM_TEMP_SIZE];
    size_t total = 0;
    size_t readCount;

    if ((m_mode & READ) == 0)
        return 0;

    // Keep looping while not hitting delimiter
    while ((readCount = read(tmpBuf, ATHENA_STREAM_TEMP_SIZE - 1)) != 0)
    {
        // Terminate string
        tmpBuf[readCount] = '\0';

        // Find first delimiter
        size_t pos = strcspn(tmpBuf, delim.c_str());

        if (pos < readCount)
        {
            // Found terminator, reposition backwards
            skip((long)(pos + 1 - readCount));

            total += pos + 1;

            // break out
            break;
        }

        total += readCount;
    }

    return total;
}

//-----------------------------------------------------------------------

template<typename T> DataStream& DataStream::operator>>(T& val)
{
    if ((m_mode & READ) == 0)
        return 0;

    read(static_cast<void*>(&val), sizeof(T));
    return *this;
}
