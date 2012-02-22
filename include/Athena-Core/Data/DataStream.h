/** @file   DataStream.h
    @author Philip Abbet

    Definition of the class 'Athena::Data::DataStream'
*/

#ifndef _ATHENA_DATA_DATASTREAM_H_
#define _ATHENA_DATA_DATASTREAM_H_

#include <Athena-Core/Prerequisites.h>


namespace Athena {
namespace Data {

//----------------------------------------------------------------------------------------
/// @brief  Base class for an object used to read and write data
///
/// This class provides basically the same methods than std::basic_istream, except that it
/// does not have any formatting capabilities, and is designed to be subclassed to receive
/// data from multiple sources, including libraries which have no compatibility with the
/// STL's stream interfaces. As such, this is an abstraction of a set of wrapper classes
/// which pretend to be standard stream classes but can actually be implemented quite
/// differently.
///
/// @remark This mecanism is a simplified version of Ogre::DataStream from
///         <a href="http://www.ogre3d.org/">Ogre3D</a>.
//----------------------------------------------------------------------------------------
class ATHENA_SYMBOL DataStream
{
    //_____ Internal types __________
public:
    enum tMode
    {
        READ = 1,
        WRITE = 2,
        READ_WRITE = READ | WRITE
    };


    //_____ Construction / Destruction __________
public:
    DataStream(tMode mode)
    : m_mode(mode)
    {
    }

    virtual ~DataStream() {};


    //_____ Methods to implement __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Read the requisite number of bytes from the stream, stopping at the end of
    ///         the file
    ///
    /// @param  buf     Reference to a buffer pointer
    /// @param  count   Number of bytes to read
    /// @return         The number of bytes read
    //------------------------------------------------------------------------------------
    virtual size_t read(void* buf, size_t count) = 0;

    //------------------------------------------------------------------------------------
    /// @brief  Write the requisite number of bytes to the stream(only applicable to
    ///         streams that are not read-only)
    ///
    /// @param  buf     Pointer to a buffer containing the bytes to write
    /// @param  count   Number of bytes to write
    /// @return         The number of bytes written
    //------------------------------------------------------------------------------------
    virtual size_t write(const void* buf, size_t count)
    {
        // Default to not supported
        return 0;
    }

    //------------------------------------------------------------------------------------
    /// @brief  Skip a defined number of bytes. This can also be a negative value, in
    ///         which case the file pointer rewinds a defined number of bytes.
    //------------------------------------------------------------------------------------
    virtual void skip(long count) = 0;

    //------------------------------------------------------------------------------------
    /// @brief  Repositions the read point to a specified byte
    //------------------------------------------------------------------------------------
    virtual void seek(size_t pos) = 0;

    //------------------------------------------------------------------------------------
    /// @brief  Returns the current byte offset from beginning
    //------------------------------------------------------------------------------------
    virtual size_t tell() = 0;

    //------------------------------------------------------------------------------------
    /// @brief  Indicates if the stream has reached the end
    //------------------------------------------------------------------------------------
    virtual bool eof() const = 0;

    //------------------------------------------------------------------------------------
    /// @brief  Close the stream; this makes further operations invalid.
    //------------------------------------------------------------------------------------
    virtual void close() = 0;


    //_____ Methods __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Get a single line from the stream
    ///
    /// @remark The delimiter character is not included in the data returned, and it is
    ///         skipped over so the next read will occur after it. The buffer contents
    ///         will include a terminating character.
    ///
    /// @remark To use this function, you <b>must</b> open the stream in <b>binary
    ///         mode</b>, otherwise, it'll produce unexpected results.
    ///
    /// @param  buf         Reference to a buffer pointer
    /// @param  maxCount    The maximum length of data to be read, excluding the
    ///                     terminating character
    /// @param  delim       The delimiter(s) to stop at
    /// @return             The number of bytes read, excluding the terminating character
    //------------------------------------------------------------------------------------
    virtual size_t readLine(char* buf, size_t maxCount, const std::string& delim = "\n");

    //------------------------------------------------------------------------------------
    /// @brief  Returns a string containing the next line of data, optionally trimmed for
    ///         whitespace
    ///
    /// @remark This is a convenience method for text streams only, allowing you to
    ///         retrieve a string object containing the next line of data. The data is
    ///         read up to the next newline character and the result trimmed if required.
    ///
    /// @remark To use this function, you <b>must</b> open the stream in <b>binary
    ///         mode</b>, otherwise, it'll produce unexpected results.
    ///
    /// @param  trimAfter   If true, the line is trimmed for whitespace
    /// @return             The next line
    //------------------------------------------------------------------------------------
    virtual std::string getLine(bool trimAfter = true);

    //------------------------------------------------------------------------------------
    /// @brief  Skip a single line from the stream
    ///
    /// @remark To use this function, you <b>must</b> open the stream in <b>binary
    ///         mode</b>, otherwise, it'll produce unexpected results.
    ///
    /// @param  delim   The delimiter(s) to stop at
    /// @return         The number of bytes skipped
    //------------------------------------------------------------------------------------
    virtual size_t skipLine(const std::string& delim = "\n");

    //------------------------------------------------------------------------------------
    /// @brief  Streaming operators
    //------------------------------------------------------------------------------------
    template<typename T> DataStream& operator>>(T& val);

    //------------------------------------------------------------------------------------
    /// @brief  Returns the mode of the stream
    //------------------------------------------------------------------------------------
    inline tMode getMode() const
    {
        return m_mode;
    }


    //_____ Attributes __________
protected:
    tMode m_mode;
};

}
}

#endif
