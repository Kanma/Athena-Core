/** @file   Describable.cpp
    @author Philip Abbet

    Implementation of the class 'Athena::Utils::Describable'
*/

#include <Athena-Core/Utils/Describable.h>


using namespace Athena;
using namespace Athena::Utils;
using namespace std;


/***************************** CONSTRUCTION / DESTRUCTION ******************************/

Describable::Describable()
: m_pUnknownProperties(0)
{
}

//-----------------------------------------------------------------------

Describable::~Describable()
{
    delete m_pUnknownProperties;
}


/**************************** MANAGEMENT OF THE PROPERTIES *****************************/

void Describable::setProperties(PropertiesList* pProperties,
                                PropertiesList* pDelayedProperties)
{
    assert(pProperties);

    PropertiesList::tCategoriesIterator catIter = pProperties->getCategoriesIterator();
    while (catIter.hasMoreElements())
    {
        PropertiesList::tPropertiesIterator propIter =
            pProperties->getPropertiesIterator(catIter.peekNextPtr()->strName);
        while (propIter.hasMoreElements())
        {
            bool bUsed = setProperty(catIter.peekNextPtr()->strName, propIter.peekNextPtr()->strName,
                                     new Variant(*(propIter.peekNextPtr()->pValue)));

            if (!bUsed && pDelayedProperties)
            {
                pDelayedProperties->set(catIter.peekNextPtr()->strName, propIter.peekNextPtr()->strName,
                                        new Variant(*(propIter.peekNextPtr()->pValue)));
            }

            propIter.moveNext();
        }

        catIter.moveNext();
    }
}

//-----------------------------------------------------------------------

bool Describable::setProperty(const string& strCategory, const string& strName, Variant* pValue)
{
    assert(!strCategory.empty());
    assert(!strName.empty());
    assert(pValue);

    // If we reach that point, the property's category isn't known by the describable, so add
    // it to the corresponding list
    if (!m_pUnknownProperties)
        m_pUnknownProperties = new PropertiesList();

    m_pUnknownProperties->set(strCategory, strName, pValue);

    return true;
}
