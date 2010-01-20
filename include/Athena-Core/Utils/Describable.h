/**	@file	Describable.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Utils::Describable'
*/

#ifndef _ATHENA_UTILS_DESCRIBABLE_H
#define _ATHENA_UTILS_DESCRIBABLE_H

#include <Athena-Core/Prerequisites.h>
#include <Athena-Core/Utils/PropertiesList.h>

namespace Athena {
namespace Utils {

//---------------------------------------------------------------------------------------
/// @brief	Base class for all the objects that can have a description
/// 
/// A description is a collection of properties, organized in sorted categories. A
///	property is a named value (of any type). The interpretation of the properties is
///	left to the describable object.
///
/// This can for example be used to populate a 'properties list' in an editor, or
/// used by an exporter/importer instead of hardcoding the specifics of each object's
/// class.
///
/// The classes that inherits from this one must implement some methods, by following
/// the indications found in their documentation. The order of the categories of
/// properties depends of the inheritance tree of the object, with the categories
/// containing the properties of the last derived class coming first.
///
/// A list of 'unknown properties' holds all the properties that were set on the
/// object, but not in a category that it can handle. The goal of this system is to
/// not lose any property in an import/export sequence when an error occured. Note
///	that it is assumed that those 'unknown categories' are the first of the list (ie.
/// if a category is known by one of the class in the inheritance tree of the object,
/// all the following categories are known by this class or by one of its base ones).
//---------------------------------------------------------------------------------------
class ATHENA_SYMBOL Describable
{
	//_____ Construction / Destruction __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief	Constructor
    //-----------------------------------------------------------------------------------
	Describable();

    //-----------------------------------------------------------------------------------
    /// @brief	Destructor
    //-----------------------------------------------------------------------------------
	virtual ~Describable();


	//_____ Management of the properties __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Returns a list containing the properties of the describable
	///
	/// @remark	Must be overriden by each describable type. Each implementation must first
	///			call its base class one, and add a new category AT THE BEGINNING of the
	///			obtained list, containing the properties related to this describable type.
	/// @return	The list of properties
	//-----------------------------------------------------------------------------------
	virtual Utils::PropertiesList* getProperties() const { return new PropertiesList(); }

	//-----------------------------------------------------------------------------------
	/// @brief	Returns a list containing the unknown properties of the describable
	/// @return	The list of unknown properties
	//-----------------------------------------------------------------------------------
	inline Utils::PropertiesList* getUnknownProperties() const { return m_pUnknownProperties; }

    //-----------------------------------------------------------------------------------
    /// @brief	Set the values of the properties of the describable from a list of
    ///         properties
    ///
    ///	@param	pProperties			The list containing the new values
    ///	@retval	pDelayedProperties	If provided, the properties that aren't usable yet
    ///                             (because, for example, another object which isn't
    ///                             already created is needed) are put into that list by
    ///                             the describable
    //-----------------------------------------------------------------------------------
	void setProperties(PropertiesList* pProperties, PropertiesList* pDelayedProperties = 0);
	
    //-----------------------------------------------------------------------------------
    /// @brief	Set the value of a property of the describable
    ///
    ///	@param	strCategory		The category of the property
    ///	@param	strName			The name of the property
    ///	@param	pValue			The value of the property
    ///	@return					'true' if the property was used, 'false' if a required
    ///                         object is missing (@see the 'pDelayedProperties' in the
    ////                        setProperties method)
    /// @remark	Must be overriden by each describable type. Each implementation must test
    ///         if the property's category is the one of the describable's type, and if
    ///         so process the property's value. Otherwise, it must call its base class
    ///         implementation.
    //-----------------------------------------------------------------------------------
	virtual bool setProperty(const std::string& strCategory, const std::string& strName,
							 Variant* pValue);


	//_____ Attributes __________
protected:
	PropertiesList*  m_pUnknownProperties;	///< List of the properties that aren't known by the describable object
};

}
}

#endif
