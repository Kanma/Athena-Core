/**	@file	XMLLogListener.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Log::XMLLogListener'
*/

#include <Athena-Core/Log/XMLLogListener.h>
#include <Athena-Core/Log/LogManager.h>

using namespace Athena::Log;
using namespace std;


/************************************** CONSTANTS ***************************************/

/// Context used for logging
static const char*	__CONTEXT__ = "XML Log listener";


/****************************** CONSTRUCTION / DESTRUCTION ******************************/

XMLLogListener::XMLLogListener(const std::string& strFileName)
: m_ulID(0)
{
	// Assertions
	assert(LogManager::getSingletonPtr() && "There isn't any LogManager instance");
	assert(!strFileName.empty() && "The file's name is empty");

	// Declarations
	time_t  startTimestamp;
	tm*		pCurrentTime;
	char	strTime[9];
	char	strDate[30];

	// Open the log file
	m_file.open(strFileName.c_str());

	// If successful, write the header
	if (m_file.is_open())
	{
		// Retrieve the current time
		startTimestamp		= time(0);
		pCurrentTime		= gmtime(&startTimestamp);
		strftime(strTime, 9, "%H:%M:%S", pCurrentTime);
		strftime(strDate, 30, "%#x", pCurrentTime);


		m_file << "<?xml version=\"1.0\" ?>" << endl
			   << "<?xml-stylesheet type=\"text/xsl\" href=\"ComplexXSLT.xsl\" ?>" << endl
			   << "<!-- Written using Athena's Log manager, by Philip Abbet -->" << endl
			   << "<RunTimeLog>" << endl
			   << "    <LogHeader>" << endl
#if ATHENA_CORE_DETAILED_LOGS
			   << "        <OutputLevel>Detailed</OutputLevel>" << endl
#else
			   << "        <OutputLevel>Normal</OutputLevel>" << endl
#endif
			   << "        <Session>" << endl
			   << "            <Started>" << endl
			   << "                <Time>" << strTime << " GMT</Time>" << endl
			   << "                <Date>" << strDate << "</Date>" << endl
			   << "            </Started>" << endl
			   << "            <Configuration>" << endl
			   << "                <Environment>...</Environment>" << endl
			   << "                <Processor>" << endl
			   << "                    <Family>...</Family>" << endl
			   << "                    <Vendor>...</Vendor>" << endl
			   << "                    <Features>...</Features>" << endl
			   << "                    <ClockSpeed>...</ClockSpeed>" << endl
			   << "                </Processor>" << endl
			   << "                <Memory>" << endl
			   << "                    <Total>...</Total>" << endl
			   << "                    <Available>...</Available>" << endl
			   << "                </Memory>" << endl
			   << "            </Configuration>" << endl
			   << "        </Session>" << endl
			   << "    </LogHeader>" << endl;

		ATHENA_LOG_EVENT("Beginning of logging in file '" + strFileName + "'");
	}
	else
	{
		ATHENA_LOG_ERROR("Failed to create the file '" + strFileName + "'");
	}
}

//-----------------------------------------------------------------------

XMLLogListener::~XMLLogListener()
{
	if (m_file.is_open())
	{
		m_file << "</RunTimeLog>" << endl;
		m_file.close();
	}
}


/*************************************** METHODS ****************************************/

bool XMLLogListener::isFileOpen() const
{
	return m_file.is_open();
}


/********************************* METHODS TO IMPLEMENT *********************************/

void XMLLogListener::log(const std::string& strTimestamp, tMessageType type,
                         const char* strContext, const std::string& strMessage,
                         const char* strFileName, const char* strFunction,
                         unsigned int uiLine)
{
	// Assertions
	assert(LogManager::getSingletonPtr() && "There isn't any LogManager instance");
	assert(m_file.is_open() && "The file isn't open");

	// Declarations
	string	strProcessedMessage;
	size_t	offset;

	// Increase the messages' counter
	++m_ulID;

	// Replace some special characters by escape codes
	strProcessedMessage = strMessage;
	
	offset = 0;
	while ((offset = strProcessedMessage.find("<", offset)) != string::npos)
		strProcessedMessage.replace(offset, 1,  "&lt;");

	offset = 0;
	while ((offset = strProcessedMessage.find(">", offset)) != string::npos)
		strProcessedMessage.replace(offset, 1,  "&gt;");


	// Write the message in the file
	m_file << "    <LogEvent id=\"" << m_ulID << "\">" << endl
		   << "        <Type>";
	switch (type)
	{
	case LOG_COMMENT:	m_file << "Comment"; break;
	case LOG_DEBUG:		m_file << "Debug"; break;
	case LOG_WARNING:	m_file << "Warning"; break;
	case LOG_ERROR:		m_file << "Error"; break;
	case LOG_EVENT:		m_file << "Event"; break;
	}
	m_file << "</Type>" << endl
		   << "        <TimeIndex>" << strTimestamp << "</TimeIndex>" << endl
		   << "        <Context>" << strContext << "</Context>" << endl
		   << "        <File>" << strFileName << "</File>" << endl
		   << "        <Function>" << strFunction << "</Function>" << endl
		   << "        <Line>" << uiLine << "</Line>" << endl
		   << "        <Message>" << strProcessedMessage << "</Message>" << endl
		   << "    </LogEvent>" << endl;
}
