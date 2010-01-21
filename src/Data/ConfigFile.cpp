/**	@file	ConfigFile.cpp
	@author	Philip Abbet

	Implementation of the class 'Athena::Data::ConfigFile'
*/

#include <Athena-Core/Data/ConfigFile.h>
#include <Athena-Core/Log/LogManager.h>
#include <Athena-Core/Data/FileDataStream.h>
#include <Athena-Core/Utils/StringConverter.h>
#include <sstream>


using namespace Athena;
using namespace Athena::Data;
using namespace Athena::Log;
using namespace Athena::Utils;
using namespace std;


/************************************** CONSTANTS **************************************/

/// Context used for logging
static const char*	__CONTEXT__ = "Configuration file";


/****************************** CONSTRUCTION / DESTRUCTION *****************************/

ConfigFile::ConfigFile()
: m_strFilename(""), m_pSelectedSection(0)
{
}


//---------------------------------------------------------------------------------------
/// @brief	Destructor
//---------------------------------------------------------------------------------------
ConfigFile::~ConfigFile()
{
	unload();
}


/*********************************** LOADING / SAVING **********************************/

bool ConfigFile::load(const std::string& strFilename)
{
    // Assertions
    assert(!strFilename.empty());

    // Open the file
    FileDataStream stream(strFilename);
    if (!stream.isOpen())
    {
		ATHENA_LOG_ERROR("Failed to load the file '" + strFilename + "'");
        return false;
    }

    // Process its content
    bool bResult = load(&stream);

    stream.close();
    
    m_strFilename = strFilename;
    
    return bResult;
}

//-----------------------------------------------------------------------

bool ConfigFile::load(DataStream* pStream)
{
    // Assertions
    assert(pStream);

	// Declarations
	string			strLine;
	tParameter		parameter;
	size_t			offset;
	tSection		section;
	tSection*		pLastSection = 0;
	char			pBuffer[255];

	// Unload the config file first
	unload();
    m_strFilename = "";

	// Parse the file
	while (!pStream->eof())
	{
		// Read the next line
		strLine = pStream->getLine();

		// Suppress the '\r' character at the end of the line
		offset = strLine.find_last_not_of("\r");
		if (offset == string::npos)
			continue;
		else if (offset != strLine.length() - 1)
			strLine.erase(offset + 1);

		// If the line is empty, go to the next one
		if (strLine.empty())
			continue;

		// Suppress the comments
		offset = strLine.find("#");
		if (offset == 0)
			continue;
		else if (offset != string::npos)
			strLine.erase(offset);

		// Suppress the leading spaces
		offset = strLine.find_first_not_of(" \t");
		if (offset == string::npos)
			continue;
		else if (offset != 0)
			strLine.erase(0, offset);
		offset = strLine.find_last_not_of(" \t");
		if (offset == string::npos)
			continue;	// Should not happen
		else if (offset != strLine.length() - 1)
			strLine.erase(offset + 1);

		// Test if the line is a section header
		if ((strLine.at(0) == '[') && (strLine.at(strLine.size() - 1) == ']'))
		{
			section.strName = strLine.substr(1, strLine.size() - 2);
			section.parameters.clear();
			m_sections.push_back(section);
			pLastSection = &(*m_sections.rbegin());
		}

		// The line should be a 'Parameter = value' one
		else if ((offset = strLine.find("=")) != string::npos)
		{
			// If we didn't add a section yet, create an unnamed one
			if (!pLastSection)
			{
				section.strName = "";
				section.parameters.clear();
				m_sections.push_back(section);
				pLastSection = &(*m_sections.rbegin());
			}

			// Initialize the parameter
			parameter.strName = strLine.substr(0, offset);
			size_t offset2 = parameter.strName.find_last_not_of(" \t");
			if (offset2 != parameter.strName.length() - 1)
				parameter.strName.erase(offset2 + 1);

			parameter.strValue = strLine.substr(offset + 1, strLine.size() - offset - 1);
			offset2 = parameter.strValue.find_first_not_of(" \t");
			if ((offset2 != string::npos) && (offset2 != 0))
				parameter.strValue.erase(0, offset2);
			
			// Add the parameter in the list
			pLastSection->parameters.push_back(parameter);
		}
	}

	return true;
}

//-----------------------------------------------------------------------

bool ConfigFile::reload()
{
	// Check that a file is already loaded
	if (m_strFilename.empty())
    {
		ATHENA_LOG_ERROR("Can't reload: the filename is unknown");
        return false;
    }

	string strFilename = m_strFilename;

	unload();

	// Reload the file
	return load(strFilename);
}

//-----------------------------------------------------------------------

bool ConfigFile::save(const std::string& strFilename)
{
    // Assertions
    assert(!strFilename.empty());

    // Declarations
    string strPath = strFilename;

    // Create the file
	if (strFilename.empty())
	{
		if (m_strFilename.empty())
		{
			ATHENA_LOG_ERROR("Can't save: no filename supplied");
			return false;
		}

        strPath = m_strFilename;
	}

    FileDataStream stream(strPath, DataStream::WRITE);
    if (!stream.isOpen())
    {
		ATHENA_LOG_ERROR("Failed to create the file '" + strPath + "'");
        return false;
    }

    // Save the parameters
    bool bResult = save(&stream);

    stream.close();
    
    if (m_strFilename.empty())
        m_strFilename = strPath;
    
    return bResult;
}

//-----------------------------------------------------------------------

bool ConfigFile::save(DataStream* pStream)
{
    // Assertions
    assert(pStream);

    // Declarations
    stringstream s;
    string s2;
    
	// Write the sections to the file
    tSectionsIterator iterSections(m_sections);
	while (iterSections.hasMoreElements())
	{
        tSection* pSection = iterSections.peekNextPtr();
	    
        s << "[" << pSection->strName << "]" << endl;
        
        s2 = s.str();
        pStream->write(s2.c_str(), s2.size());
        s.str("");

        tParametersIterator iterParam(pSection->parameters);
    	while (iterParam.hasMoreElements())
    	{
            tParameter* pParameter = iterParam.peekNextPtr();
            
			s << pParameter->strName << " = " << pParameter->strValue << endl;

            s2 = s.str();
            pStream->write(s2.c_str(), s2.size());
            s.str("");
    		
            iterParam.moveNext();
		}

		s << endl;

        s2 = s.str();
        pStream->write(s2.c_str(), s2.size());
        s.str("");
		
        iterSections.moveNext();
	}

	return true;
}

//-----------------------------------------------------------------------

void ConfigFile::unload()
{
	// Clear the sections
    tSectionsIterator iterSections(m_sections);
	while (iterSections.hasMoreElements())
	{
		iterSections.peekNextPtr()->parameters.clear();
		iterSections.moveNext();
	}
	m_sections.clear();

	// Reset the attributes
	m_strFilename		= "";
	m_pSelectedSection	= 0;
}


/****************************** PARAMETERS-RELATED METHODS ******************************/

bool ConfigFile::getParameterValue(const std::string &strSection,
								   const std::string &strParameter,
								   std::string &strValue)
{
	// Declarations
	tSectionsNativeIterator		iter, iterEnd;
	tParametersNativeIterator	paramIter, paramIterEnd;

	// Search the section
	for (iter = m_sections.begin(), iterEnd = m_sections.end(); iter != iterEnd; ++iter)
	{
		if (iter->strName == strSection)
		{
			// Search the first parameter with the specified name
			paramIter = iter->parameters.begin();
			paramIterEnd = iter->parameters.end();
			for (paramIter = iter->parameters.begin(), paramIterEnd = iter->parameters.end();
				 paramIter != paramIterEnd; ++paramIter)
			{
				if (paramIter->strName == strParameter)
				{
					strValue = paramIter->strValue;
					return true;
				}
			}

			return false;
		}
	}

	return false;
}

//-----------------------------------------------------------------------

bool ConfigFile::getParameterValue(const std::string &strParameter, std::string &strValue)
{
	// Declarations
	tParametersNativeIterator	paramIter, paramIterEnd;

	// Check that a section is selected
	if (!m_pSelectedSection)
		return false;

	// Search the first parameter with the specified name
	for (paramIter = m_pSelectedSection->parameters.begin(),
		 paramIterEnd = m_pSelectedSection->parameters.end();
		 paramIter != paramIterEnd; ++paramIter)
	{
		if (paramIter->strName == strParameter)
		{
			strValue = paramIter->strValue;
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------

bool ConfigFile::getParameterValue(const std::string &strParameter, unsigned int &uiValue)
{
	// Declarations
	string strValue;

	// Retrieve the value
	if (!getParameterValue(strParameter, strValue))
		return false;

	uiValue = StringConverter::parseUnsignedInt(strValue);
	return true;
}

//-----------------------------------------------------------------------

bool ConfigFile::getParameterValue(const std::string &strParameter, int &iValue)
{
	// Declarations
	string strValue;

	// Retrieve the value
	if (!getParameterValue(strParameter, strValue))
		return false;

	iValue = StringConverter::parseInt(strValue);
	return true;
}

//-----------------------------------------------------------------------

bool ConfigFile::getParameterValue(const std::string &strParameter, float &fValue)
{
	// Declarations
	string strValue;

	// Retrieve the value
	if (!getParameterValue(strParameter, strValue))
		return false;

	fValue = StringConverter::parseReal(strValue);
	return true;
}

//-----------------------------------------------------------------------

bool ConfigFile::getParameterValue(const std::string& strParameter, bool &bValue)
{
	// Declarations
	string strValue;

	// Retrieve the value
	if (!getParameterValue(strParameter, strValue))
		return false;

	bValue = StringConverter::parseBool(strValue);
	return true;
}

//-----------------------------------------------------------------------

bool ConfigFile::hasParameter(const std::string& strParameter)
{
	// Declarations
	string strValue;

	return getParameterValue(strParameter, strValue);
}

//-----------------------------------------------------------------------

unsigned int ConfigFile::getNbParameters(const std::string& strParameter)
{
	// Declarations
	tParametersNativeIterator	paramIter, paramIterEnd;
	unsigned int					uiNbParameters	= 0;

	// Check that a section is selected
	if (!m_pSelectedSection) return 0;

	if (strParameter.empty())
		return (unsigned int) m_pSelectedSection->parameters.size();

	// Search the first parameter with the specified name
	for (paramIter = m_pSelectedSection->parameters.begin(),
		 paramIterEnd = m_pSelectedSection->parameters.end();
		 paramIter != paramIterEnd; ++paramIter)
	{
		if (paramIter->strName == strParameter)
			++uiNbParameters;
	}

	return uiNbParameters;
}

//-----------------------------------------------------------------------

bool ConfigFile::getNextParameter(std::string &strParameter, std::string &strValue)
{
	// Check that a section is selected
	if (!m_pSelectedSection) return false;

	// Check that all the parameters aren't already red
	if (m_currentParameter == m_pSelectedSection->parameters.end())
		return false;

	// Read the name and the value of the parameter
	strParameter	= m_currentParameter->strName;
	strValue		= m_currentParameter->strValue;

	// Select the next parameter (for the next time)
	++m_currentParameter;

	return true;
}

//-----------------------------------------------------------------------

bool ConfigFile::getNextParameterValue(const std::string& strParameter, std::string &strValue)
{
	// Check that a section is selected
	if (!m_pSelectedSection) return false;

	// Read the name and the value of the parameter
	while ((m_currentParameter != m_pSelectedSection->parameters.end()) &&
		   (m_currentParameter->strName != strParameter))
		++m_currentParameter;

	// Check that all the parameters aren't already red
	if (m_currentParameter == m_pSelectedSection->parameters.end())
		return false;

	strValue = m_currentParameter->strValue;

	// Select the next parameter (for the next time)
	++m_currentParameter;

	return true;
}

//-----------------------------------------------------------------------

void ConfigFile::addParameter(const std::string& strName, const std::string& strValue)
{
	// Declarations
	tParameter parameter;

	// Initialize the parameter
	parameter.strName = strName;
	parameter.strValue = strValue;
	
	// Add the parameter in the list
	m_pSelectedSection->parameters.push_back(parameter);
}

//-----------------------------------------------------------------------

void ConfigFile::addParameter(const std::string& strName, const char* strValue)
{
	// Declarations
	tParameter parameter;

	// Initialize the parameter
	parameter.strName = strName;
	parameter.strValue = strValue;
	
	// Add the parameter in the list
	m_pSelectedSection->parameters.push_back(parameter);
}

//-----------------------------------------------------------------------

void ConfigFile::addParameter(const std::string& strName, unsigned int uiValue)
{
	// Declarations
	tParameter parameter;

	// Initialize the parameter
	parameter.strName = strName;
	parameter.strValue = StringConverter::toString(uiValue);
	
	// Add the parameter in the list
	m_pSelectedSection->parameters.push_back(parameter);
}

//-----------------------------------------------------------------------

void ConfigFile::addParameter(const std::string& strName, int iValue)
{
	// Declarations
	tParameter parameter;

	// Initialize the parameter
	parameter.strName = strName;
	parameter.strValue = StringConverter::toString(iValue);
	
	// Add the parameter in the list
	m_pSelectedSection->parameters.push_back(parameter);
}

//-----------------------------------------------------------------------

void ConfigFile::addParameter(const std::string& strName, float fValue)
{
	// Declarations
	tParameter parameter;

	// Initialize the parameter
	parameter.strName = strName;
	parameter.strValue = StringConverter::toString(fValue);
	
	// Add the parameter in the list
	m_pSelectedSection->parameters.push_back(parameter);
}

//-----------------------------------------------------------------------

void ConfigFile::addParameter(const std::string& strName, bool bValue)
{
	// Declarations
	tParameter parameter;

	// Initialize the parameter
	parameter.strName = strName;
	parameter.strValue = StringConverter::toString(bValue);
	
	// Add the parameter in the list
	m_pSelectedSection->parameters.push_back(parameter);
}

//-----------------------------------------------------------------------

bool ConfigFile::modifyParameter(const std::string& strName, const std::string& strValue)
{
	// Declarations
	tParametersNativeIterator	paramIter, paramIterEnd;

	// Check that a section is selected
	if (!m_pSelectedSection) return false;

	// Search the first parameter with the specified name
	for (paramIter = m_pSelectedSection->parameters.begin(),
		 paramIterEnd = m_pSelectedSection->parameters.end();
		 paramIter != paramIterEnd; ++paramIter)
	{
		if (paramIter->strName == strName)
		{
			paramIter->strValue = strValue;
			return true;
		}
	}

	return false;
}


/******************************* SECTIONS-RELATED METHODS *******************************/

bool ConfigFile::selectSection(const std::string &strSection)
{
	// Declarations
	tSectionsNativeIterator	iter, iterEnd;

	m_pSelectedSection = 0;

	// Search the section
	for (iter = m_sections.begin(), iterEnd = m_sections.end(); iter != iterEnd; ++iter)
	{
		if (iter->strName == strSection)
		{
			m_pSelectedSection = &(*iter);
			m_currentParameter = m_pSelectedSection->parameters.begin();
			return true;
		}
	}

	return false;
}

//-----------------------------------------------------------------------

bool ConfigFile::selectSection(const std::string& strSection, unsigned int uiIndex)
{
	// Declarations
	tSectionsNativeIterator	iter, iterEnd;
	unsigned int				uiCounter		= 0;

	// Search the sections
	for (iter = m_sections.begin(), iterEnd = m_sections.end(); iter != iterEnd; ++iter)
	{
		if (iter->strName == strSection)
		{
			if (uiCounter == uiIndex)
			{
				m_pSelectedSection = &(*iter);
				m_currentParameter = m_pSelectedSection->parameters.begin();
				return true;
			}
			++uiCounter;
		}
	}

	return false;
}

//-----------------------------------------------------------------------

unsigned int ConfigFile::getNbSections(const std::string& strSection)
{
	// Declarations
	tSectionsNativeIterator	iter, iterEnd;
	unsigned int				uiNbSections	= 0;

	if (strSection.empty())
		return (unsigned int) m_sections.size();

	// Search the sections
	for (iter = m_sections.begin(), iterEnd = m_sections.end(); iter != iterEnd; ++iter)
	{
		if (iter->strName == strSection)
			++uiNbSections;
	}

	return uiNbSections;
}

//-----------------------------------------------------------------------

bool ConfigFile::selectNextSection(std::string &strSection)
{
	// Declarations
	tSectionsNativeIterator	iter, iterEnd;

	if (!m_pSelectedSection)
	{
		m_pSelectedSection = &m_sections.front();
		strSection = m_pSelectedSection->strName;
		m_currentParameter = m_pSelectedSection->parameters.begin();
		return true;
	}

	// Search the currently selected section
	for (iter = m_sections.begin(), iterEnd = m_sections.end(); iter != iterEnd; ++iter)
	{
		if (m_pSelectedSection == &(*iter))
		{
			++iter;
			if (iter != iterEnd)
			{
				m_pSelectedSection = &(*iter);
				strSection = m_pSelectedSection->strName;
				m_currentParameter = m_pSelectedSection->parameters.begin();
				return true;
			}
			else
			{
				m_pSelectedSection = 0;
				return false;
			}
		}
	}

	return false;
}

//-----------------------------------------------------------------------

void ConfigFile::addSection(const std::string& strSection, bool bAfterSelectedSection)
{
	// Declarations
	tSection section;
	tSectionsNativeIterator	iter, iterEnd;

	section.strName = strSection;
	if (!bAfterSelectedSection || !m_pSelectedSection)
	{
		m_sections.push_back(section);
		m_pSelectedSection = &(*m_sections.rbegin());
	}
	else
	{
		// Search the currently selected section
		for (iter = m_sections.begin(), iterEnd = m_sections.end(); iter != iterEnd; ++iter)
		{
			if (m_pSelectedSection == &(*iter))
			{
				++iter;
				m_pSelectedSection = &(*m_sections.insert(iter, section));
				return;
			}
		}

		assert(false);
	}
}
