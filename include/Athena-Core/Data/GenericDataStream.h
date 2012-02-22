/** @file   GenericDataStream.h
    @author Philip Abbet

    Definition of the class 'Athena::Data::GenericDataStream'
*/

#ifndef _ATHENA_DATA_GENERICDATASTREAM_H_
#define _ATHENA_DATA_GENERICDATASTREAM_H_

#include <Athena-Core/Prerequisites.h>
#include <Athena-Core/Data/DataStream.h>


namespace Athena {
namespace Data {

//----------------------------------------------------------------------------------------
/// @brief    Template for a DataStream implementation using any stream-like object
//----------------------------------------------------------------------------------------
template<typename T>
class GenericDataStream: public DataStream
{
    //_____ Construction / Destruction __________
public:
    //------------------------------------------------------------------------------------
    /// @brief    Constructor
    ///
    /// @param  strFileName     Path to the file
    /// @param  bReadOnly       Indicates if the file must be open in read-only mode
    //------------------------------------------------------------------------------------
    GenericDataStream(T* stream, tMode mode)
    : DataStream(mode), m_stream(stream)
    {
    }

    //------------------------------------------------------------------------------------
    /// @brief    Destructor
    //------------------------------------------------------------------------------------
    virtual ~GenericDataStream() {}


    //_____ Implementation of DataStream __________
public:
    //------------------------------------------------------------------------------------
    /// @brief    Read the requisite number of bytes from the stream, stopping at the end of
    ///         the file
    ///
    /// @param  buf     Reference to a buffer pointer
    /// @param  count   Number of bytes to read
    /// @return         The number of bytes read
    //------------------------------------------------------------------------------------
    virtual size_t read(void* buf, size_t count)
    {
        return m_stream->read(buf, count);
    }

    //------------------------------------------------------------------------------------
    /// @brief    Write the requisite number of bytes to the stream(only applicable to
    ///         streams that are not read-only)
    ///
    /// @param  buf     Pointer to a buffer containing the bytes to write
    /// @param  count   Number of bytes to write
    /// @return         The number of bytes written
    //------------------------------------------------------------------------------------
    virtual size_t write(const void* buf, size_t count)
    {
        return m_stream->write(buf, count);
    }

    //------------------------------------------------------------------------------------
    /// @brief    Skip a defined number of bytes. This can also be a negative value, in
    ///         which case the file pointer rewinds a defined number of bytes.
    //------------------------------------------------------------------------------------
    virtual void skip(long count)
    {
        m_stream->skip(count);
    }

    //------------------------------------------------------------------------------------
    /// @brief    Repositions the read point to a specified byte
    //------------------------------------------------------------------------------------
    virtual void seek(size_t pos)
    {
        m_stream->seek(pos);
    }

    //------------------------------------------------------------------------------------
    /// @brief    Returns the current byte offset from beginning
    //------------------------------------------------------------------------------------
    virtual size_t tell()
    {
        m_stream->tell();
    }

    //------------------------------------------------------------------------------------
    /// @brief    Indicates if the stream has reached the end
    //------------------------------------------------------------------------------------
    virtual bool eof() const
    {
        return m_stream->eof();
    }

    //------------------------------------------------------------------------------------
    /// @brief    Close the stream; this makes further operations invalid.
    //------------------------------------------------------------------------------------
    virtual void close()
    {
        m_stream->close();
    }


    //_____ Attributes __________
protected:
    T* m_stream;
};

}
}

#endif
