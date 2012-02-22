/** @file   FileDataStream.h
    @author Philip Abbet

    Definition of the class 'Athena::Data::FileDataStream'
*/

#ifndef _ATHENA_DATA_FILEDATASTREAM_H_
#define _ATHENA_DATA_FILEDATASTREAM_H_

#include <Athena-Core/Prerequisites.h>
#include <Athena-Core/Data/DataStream.h>
#include <fstream>


namespace Athena {
namespace Data {

//----------------------------------------------------------------------------------------
/// @brief  DataStream implementation for a file
//----------------------------------------------------------------------------------------
class ATHENA_SYMBOL FileDataStream: public DataStream
{
    //_____ Construction / Destruction __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Constructor
    ///
    /// @param  strFileName     Path to the file
    /// @param  bReadOnly       Indicates if the file must be open in read-only mode
    //------------------------------------------------------------------------------------
    FileDataStream(const std::string& strFileName, tMode mode = READ);

    //------------------------------------------------------------------------------------
    /// @brief  Destructor
    //------------------------------------------------------------------------------------
    virtual ~FileDataStream();


    //_____ Methods __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Indicates if the file was successfully opened
    //------------------------------------------------------------------------------------
    inline bool isOpen() const
    {
        return m_stream.is_open();
    }


    //_____ Implementation of DataStream __________
public:
    //------------------------------------------------------------------------------------
    /// @brief  Read the requisite number of bytes from the stream, stopping at the end of
    ///         the file
    ///
    /// @param  buf     Reference to a buffer pointer
    /// @param  count   Number of bytes to read
    /// @return         The number of bytes read
    //------------------------------------------------------------------------------------
    virtual size_t read(void* buf, size_t count);

    //------------------------------------------------------------------------------------
    /// @brief  Write the requisite number of bytes to the stream(only applicable to
    ///         streams that are not read-only)
    ///
    /// @param  buf     Pointer to a buffer containing the bytes to write
    /// @param  count   Number of bytes to write
    /// @return         The number of bytes written
    //------------------------------------------------------------------------------------
    virtual size_t write(const void* buf, size_t count);

    //------------------------------------------------------------------------------------
    /// @brief  Skip a defined number of bytes. This can also be a negative value, in
    ///         which case the file pointer rewinds a defined number of bytes.
    //------------------------------------------------------------------------------------
    virtual void skip(long count);

    //------------------------------------------------------------------------------------
    /// @brief  Repositions the read point to a specified byte
    //------------------------------------------------------------------------------------
    virtual void seek(size_t pos);

    //------------------------------------------------------------------------------------
    /// @brief  Returns the current byte offset from beginning
    //------------------------------------------------------------------------------------
    virtual size_t tell();

    //------------------------------------------------------------------------------------
    /// @brief  Indicates if the stream has reached the end
    //------------------------------------------------------------------------------------
    virtual bool eof() const;

    //------------------------------------------------------------------------------------
    /// @brief  Close the stream; this makes further operations invalid.
    //------------------------------------------------------------------------------------
    virtual void close();


    //_____ Attributes __________
protected:
    std::fstream m_stream;
};

}
}

#endif
