/** @file   PropertiesList.cpp
    @author Philip Abbet

    Implementation of the class 'Athena::Utils::PropertiesList'
*/

#include <Athena-Core/Utils/PropertiesList.h>

using namespace Athena;
using namespace Athena::Utils;
using namespace std;


static PropertiesList::tPropertiesList empty;


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

PropertiesList::PropertiesList()
{
    m_selectedCategory = m_categories.end();
}

//-----------------------------------------------------------------------

PropertiesList::~PropertiesList()
{
    tCategoriesIterator catIter(m_categories.begin(), m_categories.end());
    while (catIter.hasMoreElements())
    {
        tPropertiesList* pProperties = &(catIter.peekNextPtr()->values);

        tPropertiesIterator propIter(pProperties->begin(), pProperties->end());
        while (propIter.hasMoreElements())
            delete propIter.getNext().pValue;

        catIter.moveNext();
    }
}


/****************************** MANAGEMENT OF THE LIST *********************************/

void PropertiesList::selectCategory(const std::string& strCategory, bool bInsertAtEnd)
{
    // Assertions
    assert(!strCategory.empty());

    selectCategory(strCategory, bInsertAtEnd ? m_categories.end() : m_categories.begin());
}

//-----------------------------------------------------------------------

void PropertiesList::selectCategory(const std::string& strCategory, tCategoriesList::iterator position)
{
    // Assertions
    assert(!strCategory.empty());

    // Search the category
    for (m_selectedCategory = m_categories.begin(); m_selectedCategory != m_categories.end();
         ++m_selectedCategory)
    {
        if (m_selectedCategory->strName == strCategory)
            return;
    }

    // Not found, create it
    tCategory category;
    category.strName = strCategory;
    m_selectedCategory = m_categories.insert(position, category);
}

//-----------------------------------------------------------------------

void PropertiesList::set(const std::string& strName, Variant* pValue)
{
    // Assertions
    assert(!strName.empty());
    assert(pValue);

    // Create a default category if none is selected
    if (m_selectedCategory == m_categories.end())
        selectCategory("DEFAULT");

    // Search the value
    PropertiesList::tPropertiesIterator iter(m_selectedCategory->values.begin(),
                                             m_selectedCategory->values.end());
    while (iter.hasMoreElements())
    {
        if (iter.peekNextPtr()->strName == strName)
        {
            delete iter.peekNextPtr()->pValue;
            iter.peekNextPtr()->pValue = pValue;
            return;
        }

        iter.moveNext();
    }

    // Not found: create it
    tProperty prop;
    prop.strName = strName;
    prop.pValue = pValue;
    m_selectedCategory->values.push_back(prop);
}

//-----------------------------------------------------------------------

void PropertiesList::set(const std::string& strCategory, const std::string& strName,
                         Variant* pValue)
{
    // Assertions
    assert(!strCategory.empty());
    assert(!strName.empty());
    assert(pValue);

    // Select the category
    selectCategory(strCategory);

    // Set the value
    set(strName, pValue);
}

//-----------------------------------------------------------------------

Variant* PropertiesList::get(const std::string& strName)
{
    assert(m_selectedCategory != m_categories.end());
    assert(!strName.empty());

    // Search the value
    PropertiesList::tPropertiesIterator iter(m_selectedCategory->values.begin(), m_selectedCategory->values.end());
    while (iter.hasMoreElements())
    {
        if (iter.peekNextPtr()->strName == strName)
            return iter.peekNextPtr()->pValue;

        iter.moveNext();
    }

    // Not found
    return 0;
}

//-----------------------------------------------------------------------

Variant* PropertiesList::get(const std::string& strCategory, const std::string& strName)
{
    assert(!strCategory.empty());
    assert(!strName.empty());

    // Search the value
    PropertiesList::tPropertiesIterator iter = getPropertiesIterator(strCategory);
    while (iter.hasMoreElements())
    {
        if (iter.peekNextPtr()->strName == strName)
            return iter.peekNextPtr()->pValue;

        iter.moveNext();
    }

    // Not found
    return 0;
}

//-----------------------------------------------------------------------

void PropertiesList::remove(const std::string& strCategory, const std::string& strName)
{
    // Assertions
    assert(!strCategory.empty());
    assert(!strName.empty());

    tPropertiesList::iterator iterProp, iterPropEnd;

    // Search the category
    tCategoriesIterator iter = getCategoriesIterator();
    while (iter.hasMoreElements())
    {
        if (iter.peekNextPtr()->strName == strCategory)
        {
            tCategory* pCategory = iter.peekNextPtr();
            
            for (iterProp = pCategory->values.begin(), iterPropEnd = pCategory->values.end();
                 iterProp != iterPropEnd; ++iterProp)
            {
                if (iterProp->strName == strName)
                {
                    pCategory->values.erase(iterProp);
                    break;
                }
            }

            return;
        }

        iter.moveNext();
    }
}

//-----------------------------------------------------------------------

void PropertiesList::remove(const std::string& strName)
{
    // Assertions
    assert(m_selectedCategory != m_categories.end());
    assert(!strName.empty());

    tPropertiesList::iterator iterProp, iterPropEnd;

    // Search the property
    for (iterProp = m_selectedCategory->values.begin(), iterPropEnd = m_selectedCategory->values.end();
         iterProp != iterPropEnd; ++iterProp)
    {
        if (iterProp->strName == strName)
        {
            m_selectedCategory->values.erase(iterProp);
            break;
        }
    }
}

//-----------------------------------------------------------------------

void PropertiesList::removeEmptyCategories()
{
    tCategoriesList::iterator iter, current, iterEnd;

    for (iter = m_categories.begin(), iterEnd = m_categories.end(); iter != iterEnd; )
    {
        current = iter;
        ++iter;
        
        if (current->values.empty())
            m_categories.erase(current);
    }
}

//-----------------------------------------------------------------------

PropertiesList::tCategoriesIterator PropertiesList::getCategoriesIterator()
{
    return tCategoriesIterator(m_categories.begin(), m_categories.end());
}

//-----------------------------------------------------------------------

PropertiesList::tPropertiesIterator PropertiesList::getPropertiesIterator()
{
    assert(m_selectedCategory != m_categories.end());

    return tPropertiesIterator(m_selectedCategory->values.begin(), m_selectedCategory->values.end());
}

//-----------------------------------------------------------------------

PropertiesList::tPropertiesIterator PropertiesList::getPropertiesIterator(const std::string& strCategory)
{
    assert(!strCategory.empty());

    // Search the category
    PropertiesList::tCategoriesIterator iter = getCategoriesIterator();
    while (iter.hasMoreElements())
    {
        if (iter.peekNextPtr()->strName == strCategory)
            return tPropertiesIterator(iter.peekNextPtr()->values.begin(), iter.peekNextPtr()->values.end());

        iter.moveNext();
    }

    // Not found
    return tPropertiesIterator(empty.begin(), empty.end());
}

//-----------------------------------------------------------------------

void PropertiesList::append(PropertiesList* pList, bool bAtEnd)
{
    // Assertions
    assert(pList);

    // Declarations
    tCategoriesList::iterator position = bAtEnd ? m_categories.end() : m_categories.begin();

    PropertiesList::tCategoriesIterator iter = pList->getCategoriesIterator();
    while (iter.hasMoreElements())
    {
        // Select the category
        selectCategory(iter.peekNextPtr()->strName, position);
        position = m_selectedCategory;
        ++position;

        tPropertiesIterator propIter = tPropertiesIterator(iter.peekNextPtr()->values.begin(), iter.peekNextPtr()->values.end());
        while (propIter.hasMoreElements())
        {
            // Set the value
            set(propIter.peekNextPtr()->strName, new Variant(*(propIter.peekNextPtr()->pValue)));

            propIter.moveNext();
        }

        iter.moveNext();
    }
}

//-----------------------------------------------------------------------

unsigned int PropertiesList::nbProperties(const std::string& strCategory)
{
    assert(!strCategory.empty());

    // Search the category
    PropertiesList::tCategoriesIterator iter = getCategoriesIterator();
    while (iter.hasMoreElements())
    {
        if (iter.peekNextPtr()->strName == strCategory)
            return iter.peekNextPtr()->values.size();

        iter.moveNext();
    }

    // Not found
    return 0;
}

//-----------------------------------------------------------------------

unsigned int PropertiesList::nbTotalProperties()
{
    unsigned int nb = 0;

    // Iterate through the categories
    PropertiesList::tCategoriesIterator iter = getCategoriesIterator();
    while (iter.hasMoreElements())
    {
        nb += iter.peekNextPtr()->values.size();
        iter.moveNext();
    }

    return nb;
}
