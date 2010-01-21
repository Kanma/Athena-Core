/**	@file	FileDataStream.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Data::FileDataStream'
*/

#include <Athena-Core/Data/FileDataStream.h>

using namespace Athena::Data;
using namespace std;


/****************************** CONSTRUCTION / DESTRUCTION ******************************/

FileDataStream::FileDataStream(const std::string& strFileName, tMode mode)
: DataStream(mode)
{
    m_stream.open(strFileName.c_str(), (mode == READ ? ios_base::in :
                                                       (mode == WRITE ? ios_base::out :
                                                                        ios_base::in | ios_base::out)));
}

//-----------------------------------------------------------------------

FileDataStream::~FileDataStream()
{
    m_stream.close();
}


/**************************** IMPLEMENTATION OF DataStream ******************************/

size_t FileDataStream::read(void* buf, size_t count)
{
    if ((m_mode & READ) == 0)
        return 0;

	m_stream.read(static_cast<char*>(buf), static_cast<std::streamsize>(count));
    return m_stream.gcount();
}

//-----------------------------------------------------------------------

size_t FileDataStream::write(const void* buf, size_t count)
{
    if ((m_mode & WRITE) == 0)
        return 0;

	m_stream.write(static_cast<const char*>(buf), static_cast<std::streamsize>(count));
	return count;
}

//-----------------------------------------------------------------------

void FileDataStream::skip(long count)
{
	m_stream.clear();
	m_stream.seekg(static_cast<std::ifstream::pos_type>(count), std::ios::cur);
}

//-----------------------------------------------------------------------

void FileDataStream::seek(size_t pos)
{
	m_stream.clear();
	m_stream.seekg(static_cast<std::streamoff>(pos), std::ios::beg);
}

//-----------------------------------------------------------------------

size_t FileDataStream::tell()
{
	m_stream.clear();
	return m_stream.tellg();
}

//-----------------------------------------------------------------------

bool FileDataStream::eof() const
{
    return m_stream.eof();
}

//-----------------------------------------------------------------------

void FileDataStream::close()
{
    m_stream.close();
}
