/** @file   PropertiesList.h
    @author Philip Abbet

    Declaration of the class 'Athena::Utils::PropertiesList'
*/

#ifndef _ATHENA_UTILS_PROPERTIESLIST_H
#define _ATHENA_UTILS_PROPERTIESLIST_H

#include <Athena-Core/Prerequisites.h>
#include <Athena-Core/Utils/Variant.h>
#include <Athena-Core/Utils/Iterators.h>

namespace Athena {
namespace Utils {


//-----------------------------------------------------------------------------------
/// @brief  Represents a list of properties, optionally organized by categories
///
/// @remark    The categories and properties aren't sorted by name, so the user can rely
///            on their order if needed
//-----------------------------------------------------------------------------------
class ATHENA_CORE_SYMBOL PropertiesList
{
    //_____ Internal types __________
public:
    struct tProperty
    {
        std::string strName;
        Variant*    pValue;
    };

    typedef std::vector<tProperty>          tPropertiesList;
    typedef VectorIterator<tPropertiesList> tPropertiesIterator;

    // We don't use a map here because the order of the categories matters
    struct tCategory
    {
        std::string     strName;
        tPropertiesList values;
    };

    typedef std::vector<tCategory>          tCategoriesList;
    typedef VectorIterator<tCategoriesList> tCategoriesIterator;


    //_____ Construction / Destruction __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief  Constructor
    //-----------------------------------------------------------------------------------
    PropertiesList();

    //-----------------------------------------------------------------------------------
    /// @brief  Destructor
    //-----------------------------------------------------------------------------------
    ~PropertiesList();


    //_____ Management of the list __________
public:
    //-----------------------------------------------------------------------------------
    /// @brief  Select a category of properties
    ///
    /// @remark If the category doesn't exists, create it.
    ///
    /// @param  strCategory     Name of the category
    /// @param  bInsertAtEnd    Indicates if the created category (if any) is inserted
    ///                         at the end or at the beginning of the list
    //-----------------------------------------------------------------------------------
    void selectCategory(const std::string& strCategory, bool bInsertAtEnd = true);

    //-----------------------------------------------------------------------------------
    /// @brief  Set a value of the list
    ///
    /// @remark If the value (or the category) doesn't exists, create it.
    /// @remark The category becomes the selected one
    ///
    /// @param  strCategory     Name of the category
    /// @param  strName         Name of the value
    /// @param  pValue          The value. The list take cares of its destruction.
    //-----------------------------------------------------------------------------------
    void set(const std::string& strCategory, const std::string& strName, Variant* pValue);

    //-----------------------------------------------------------------------------------
    /// @brief  Set a value of the list, in the selected category
    ///
    /// @remark If the value doesn't exists, create it
    ///
    /// @param  strName     Name of the value
    /// @param  pValue      The value. The list take cares of its destruction.
    //-----------------------------------------------------------------------------------
    void set(const std::string& strName, Variant* pValue);

    //-----------------------------------------------------------------------------------
    /// @brief  Get a value from the list
    ///
    /// @param  strCategory    Name of the category
    /// @param  strName        Name of the value
    /// @return                The value, 0 if not found
    //-----------------------------------------------------------------------------------
    Variant* get(const std::string& strCategory, const std::string& strName);

    //-----------------------------------------------------------------------------------
    /// @brief  Get a value from the list, in the selected category
    ///
    /// @param  strName    Name of the value
    /// @return            The value, 0 if not found
    //-----------------------------------------------------------------------------------
    Variant* get(const std::string& strName);

    //-----------------------------------------------------------------------------------
    /// @brief  Returns an iterator over the categories
    ///
    /// @return The iterator
    //-----------------------------------------------------------------------------------
    tCategoriesIterator getCategoriesIterator();

    //-----------------------------------------------------------------------------------
    /// @brief  Returns an iterator over the properties of a category
    ///
    /// @return The iterator
    //-----------------------------------------------------------------------------------
    tPropertiesIterator getPropertiesIterator(const std::string& strCategory);

    //-----------------------------------------------------------------------------------
    /// @brief  Returns an iterator over the properties of the selected category
    ///
    /// @return The iterator
    //-----------------------------------------------------------------------------------
    tPropertiesIterator getPropertiesIterator();

    //-----------------------------------------------------------------------------------
    /// @brief  Append a properties list at the end (or the beginning) of this one
    ///
    /// @param  pList   The list to append
    /// @param  bAtEnd  'true' to append at the end, 'false' for the beginning
    //-----------------------------------------------------------------------------------
    void append(PropertiesList* pList, bool bAtEnd = true);

private:
    void selectCategory(const std::string& strCategory, tCategoriesList::iterator position);


    //_____ Attributes __________
private:
    tCategoriesList              m_categories;          ///< The list of categories
    tCategoriesList::iterator    m_selectedCategory;    ///< The selected category
};

}
}

#endif
