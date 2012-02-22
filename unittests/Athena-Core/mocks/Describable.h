/** @file   Describable.h
    @author Philip Abbet

    Definition of the classes 'MockDescribable1' and 'MockDescribable2'
*/

#ifndef _MOCKDESCRIBABLE_H_
#define _MOCKDESCRIBABLE_H_

#include <Athena-Core/Prerequisites.h>
#include <Athena-Core/Utils/Describable.h>


//---------------------------------------------------------------------------------------
/// @brief    Mock implementation of a describable object
//---------------------------------------------------------------------------------------
class MockDescribable1: public Athena::Utils::Describable
{
    //_____ Construction / Destruction __________
public:
    MockDescribable1()
    : strName("test")
    {
    }

    virtual ~MockDescribable1()
    {
    }


    //_____ Management of the properties __________
public:
    virtual Athena::Utils::PropertiesList* getProperties() const
    {
        Athena::Utils::PropertiesList* pList = Athena::Utils::Describable::getProperties();

        pList->selectCategory("Cat1", false);
        pList->set("name", new Athena::Utils::Variant(strName));

        return pList;
    }


    virtual bool setProperty(const std::string& strCategory, const std::string& strName,
                             Athena::Utils::Variant* pValue)
    {
        if (strCategory == "Cat1")
        {
            bool bUsed = true;

            if (strName == "name")
                this->strName = pValue->toString();

            else if (strName == "delayed")
                bUsed = false;

            delete pValue;

            return bUsed;
        }
        else
        {
            return Athena::Utils::Describable::setProperty(strCategory, strName, pValue);
        }
    }


    //_____ Attributes __________
public:
    std::string    strName;
};


//---------------------------------------------------------------------------------------
/// @brief    Mock implementation of a describable object
//---------------------------------------------------------------------------------------
class MockDescribable2: public MockDescribable1
{
    //_____ Construction / Destruction __________
public:
    MockDescribable2()
    : iIndex(10)
    {
    }

    virtual ~MockDescribable2()
    {
    }


    //_____ Management of the properties __________
public:
    virtual Athena::Utils::PropertiesList* getProperties() const
    {
        Athena::Utils::PropertiesList* pList = MockDescribable1::getProperties();

        pList->selectCategory("Cat2", false);
        pList->set("index", new Athena::Utils::Variant(iIndex));

        return pList;
    }


    virtual bool setProperty(const std::string& strCategory, const std::string& strName,
                             Athena::Utils::Variant* pValue)
    {
        if (strCategory == "Cat2")
        {
            if (strName == "index")
                iIndex = pValue->toInt();

            delete pValue;

            return true;
        }
        else
        {
            return MockDescribable1::setProperty(strCategory, strName, pValue);
        }
    }


    //_____ Attributes __________
public:
    int    iIndex;
};


#endif
