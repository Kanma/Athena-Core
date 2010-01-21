/** @file	ConfigFile.h
	@author	Philip Abbet

	Definition of the class 'Athena::Data::ConfigFile'
*/

#ifndef _ATHENA_DATA_CONFIGFILE_H_
#define _ATHENA_DATA_CONFIGFILE_H_

#include <Athena-Core/Prerequisites.h>
#include <Athena-Core/Utils/Iterators.h>


namespace Athena {
namespace Data {

//----------------------------------------------------------------------------------------
/// @brief	Represents a configuration file
/// 
/// A configuration file is separated in sections, each one containing some parameters
/// and their values (in the form 'Parameter = Value').
///
/// A section begins with a [SectionName] line.
///
/// A parameter value can be retrieved from the complete section/parameter pair, or a
/// section can be selected, from which all the unqualified parameters are retrieved.
/// It is also possible to iterate through all the parameters and sections.
//----------------------------------------------------------------------------------------
class ATHENA_SYMBOL ConfigFile
{
	//_____ Construction / Destruction __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Constructor
    //------------------------------------------------------------------------------------
	ConfigFile();

    //------------------------------------------------------------------------------------
    /// @brief	Destructor
    //------------------------------------------------------------------------------------
	~ConfigFile();


	//_____ Loading / Saving __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Load a configuration file
    ///
    /// @param	strFilename		Name of the file
    /// @return					'true' if successful
    //------------------------------------------------------------------------------------
	bool load(const std::string& strFilename);

    //------------------------------------------------------------------------------------
    /// @brief	Load a configuration file from a data stream
    ///
    /// @param	pStream     The stream
    /// @return				'true' if successful
    ///
    /// @remark The reading of the content of the stream will begin at its current
    ///         position
    //------------------------------------------------------------------------------------
	bool load(DataStream* pStream);
	
    //------------------------------------------------------------------------------------
    /// @brief	Reload a configuration file
    /// @return	'true' if successful
    /// @remark Only works for files loaded using the filename as a parameter
    //------------------------------------------------------------------------------------
	bool reload();
	
    //------------------------------------------------------------------------------------
    /// @brief	Save the configuration file
    /// @param	strFilename     Name of the file to write, if empty use the same one than
    ///                         when the configuration file was loaded
    /// @return				    'true' if successful
    //------------------------------------------------------------------------------------
	bool save(const std::string& strFilename = "");

    //------------------------------------------------------------------------------------
    /// @brief	Save the configuration file in a data stream
    ///
    /// @param	pStream     The stream
    /// @return				'true' if successful
    ///
    /// @remark The writing will begin at the current position of the stream
    //------------------------------------------------------------------------------------
	bool save(DataStream* pStream);

    //------------------------------------------------------------------------------------
    /// @brief	Unload the configuration file
    //------------------------------------------------------------------------------------
	void unload();


	//_____ Parameters-related methods __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Returns the value of a parameter
    /// 
    /// @param	strSection		Name of the section in which to search the parameter
    /// @param	strParameter	Name of the parameter
    /// @retval	strValue		Value of the parameter
    /// @return					'true' if successful
    //------------------------------------------------------------------------------------
	bool getParameterValue(const std::string& strSection, const std::string& strParameter,
						   std::string &strValue);

    //------------------------------------------------------------------------------------
    /// @brief	Returns the value of a parameter of the currently selected section
    /// 
    /// @param	strParameter	Name of the parameter
    /// @retval	strValue		Value of the parameter
    /// @return					'true' if successful
    //------------------------------------------------------------------------------------
	bool getParameterValue(const std::string& strParameter, std::string &strValue);

    //------------------------------------------------------------------------------------
    /// @brief	Returns the value of a parameter of the currently selected section
    /// 
    /// @param	strParameter	Name of the parameter
    /// @retval	uiValue			Value of the parameter
    /// @return					'true' if successful
    //------------------------------------------------------------------------------------
	bool getParameterValue(const std::string& strParameter, unsigned int &uiValue);

    //------------------------------------------------------------------------------------
    /// @brief	Returns the value of a parameter of the currently selected section
    /// 
    /// @param	strParameter	Name of the parameter
    /// @retval	iValue			Value of the parameter
    /// @return					'true' if successful
    //------------------------------------------------------------------------------------
	bool getParameterValue(const std::string& strParameter, int &iValue);

    //------------------------------------------------------------------------------------
    /// @brief	Returns the value of a parameter of the currently selected section
    /// 
    /// @param	strParameter	Name of the parameter
    /// @retval	fValue			Value of the parameter
    /// @return					'true' if successful
    //------------------------------------------------------------------------------------
	bool getParameterValue(const std::string& strParameter, float &fValue);

    //------------------------------------------------------------------------------------
    /// @brief	Returns the value of a parameter of the currently selected section
    /// 
    /// @param	strParameter	Name of the parameter
    /// @retval	bValue			Value of the parameter
    /// @return					'true' if successful
    //------------------------------------------------------------------------------------
	bool getParameterValue(const std::string& strParameter, bool &bValue);

    //------------------------------------------------------------------------------------
    /// @brief	Indicates if a parameter exists in the currently selected section
    /// 
    /// @param	strParameter	Name of the parameter
    /// @return					'true' if the parameter exists
    //------------------------------------------------------------------------------------
	bool hasParameter(const std::string& strParameter);

    //------------------------------------------------------------------------------------
    /// @brief	Returns the number of parameters with the specified name in the selected
    ///			section
    /// 
    /// @param	strParameter	Name of the parameters to count (blank to count all the
    ///							parameters)
    /// @return					'true' if successful
    //------------------------------------------------------------------------------------
	unsigned int getNbParameters(const std::string& strParameter = "");

    //------------------------------------------------------------------------------------
    /// @brief	Get the next parameter in the selected section
    ///
    /// @retval	strParameter	The name of the parameter
    /// @retval	strValue		The value of the parameter
    /// @return					'true' if successful
    //------------------------------------------------------------------------------------
	bool getNextParameter(std::string &strParameter, std::string &strValue);

    //------------------------------------------------------------------------------------
    /// @brief	Get the value of the next parameter with the specified name
    ///
    /// @param	strParameter	The name of the parameter
    /// @retval	strValue		The value of the parameter
    /// @return					'true' if successful
    //------------------------------------------------------------------------------------
	bool getNextParameterValue(const std::string& strParameter, std::string &strValue);

    //------------------------------------------------------------------------------------
    /// @brief	Add a new parameter in the currently selected section
    ///
    /// @param	strName		The name of the parameter
    /// @param	strValue	The value of the parameter
    //------------------------------------------------------------------------------------
	void addParameter(const std::string& strName, const std::string& strValue);

    //------------------------------------------------------------------------------------
    /// @brief	Add a new parameter in the currently selected section
    ///
    /// @param	strName		The name of the parameter
    /// @param	strValue	The value of the parameter
    //------------------------------------------------------------------------------------
	void addParameter(const std::string& strName, const char* strValue);

    //------------------------------------------------------------------------------------
    /// @brief	Add a new parameter in the currently selected section
    ///
    /// @param	strName		The name of the parameter
    /// @param	uiValue		The value of the parameter
    //------------------------------------------------------------------------------------
	void addParameter(const std::string& strName, unsigned int uiValue);

    //------------------------------------------------------------------------------------
    /// @brief	Add a new parameter in the currently selected section
    ///
    /// @param	strName		The name of the parameter
    /// @param	iValue		The value of the parameter
    //------------------------------------------------------------------------------------
	void addParameter(const std::string& strName, int iValue);

    //------------------------------------------------------------------------------------
    /// @brief	Add a new parameter in the currently selected section
    ///
    /// @param	strName		The name of the parameter
    /// @param	fValue		The value of the parameter
    //------------------------------------------------------------------------------------
	void addParameter(const std::string& strName, float fValue);

    //------------------------------------------------------------------------------------
    /// @brief	Add a new parameter in the currently selected section
    ///
    /// @param	strName		The name of the parameter
    /// @param	bValue		The value of the parameter
    //------------------------------------------------------------------------------------
	void addParameter(const std::string& strName, bool bValue);

    //------------------------------------------------------------------------------------
    /// @brief	Modify the value of a parameter in the currently selected section
    ///
    /// @param	strName		The name of the parameter
    /// @param	strValue	The new value of the parameter
    //------------------------------------------------------------------------------------
	bool modifyParameter(const std::string& strName, const std::string& strValue);


	//_____ Sections-related methods __________
public:
    //------------------------------------------------------------------------------------
    /// @brief	Select a section
    /// 
    /// @param	strSection	Name of the section
    /// @return				'true' if successful
    //------------------------------------------------------------------------------------
	bool selectSection(const std::string& strSection);

    //------------------------------------------------------------------------------------
    /// @brief	Select a section
    /// 
    /// @param	strSection	Name of the section
    /// @param	uiIndex		Index of the section (the n-th section with that name)
    /// @return				'true' if successful
    //------------------------------------------------------------------------------------
	bool selectSection(const std::string& strSection, unsigned int uiIndex);

    //------------------------------------------------------------------------------------
    /// @brief	Returns the number of sections with the specified name
    /// 
    /// @param	strSection	Name of the sections to count (blank to count all the sections)
    /// @return				'true' if successful
    //------------------------------------------------------------------------------------
	unsigned int getNbSections(const std::string& strSection = "");

    //------------------------------------------------------------------------------------
    /// @brief	Select the next section with the specified name
    ///
    /// @param	strSection	Name of the section (blank to select the next section)
    /// @return				'true' if successful
    //------------------------------------------------------------------------------------
	bool selectNextSection(std::string &strSection);

    //------------------------------------------------------------------------------------
    /// @brief	Add a new section
    ///
    /// @remark	The new section become the selected one
    /// @param	strSection				The name of the section
    /// @retval	bAfterSelectedSection	Indicates if the section must be put after the
    ///									currently selected one
    //------------------------------------------------------------------------------------
	void addSection(const std::string& strSection, bool bAfterSelectedSection = false);


	//_____ Internal types ___________
private:
	//-----------------------------------------------------------------------------------
	/// @brief	Represents a parameter
	//-----------------------------------------------------------------------------------
	struct tParameter
	{
		std::string strName;	///< Name of the parameter
		std::string strValue;	///< Value of the parameter
	};

    typedef std::vector<tParameter>                 tParametersList;        ///< A list of parameters
    typedef Utils::VectorIterator<tParametersList>  tParametersIterator; 
    typedef tParametersList::iterator               tParametersNativeIterator; 

	//-----------------------------------------------------------------------------------
	/// @brief	Represents a section
	//-----------------------------------------------------------------------------------
	struct tSection
	{
		std::string	    strName;	///< Name of the section
		tParametersList parameters;	///< List of the parameters in the section
	};

    typedef std::vector<tSection>                   tSectionsList;        ///< A list of sections
    typedef Utils::VectorIterator<tSectionsList>    tSectionsIterator; 
    typedef tSectionsList::iterator                 tSectionsNativeIterator; 


	//_____ Attributes ___________
private:
	std::string			        m_strFilename;		///< Name of the file
	tSectionsList	            m_sections;			///< List of the sections
	tSection*			        m_pSelectedSection;	///< Section currently selected
	tParametersNativeIterator	m_currentParameter;	///< Parameter currently selected
};

}
}

#endif
