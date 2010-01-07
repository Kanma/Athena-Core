#include <UnitTest++.h>
#include <Athena-Core/Iterators.h>

using namespace Athena;
using namespace Athena::Utils;
using namespace std;


SUITE(VectorIteratorTests)
{
    TEST(FullIteration)
    {
        vector<int> v;
        
        v.push_back(0);
        v.push_back(1);
        v.push_back(2);

        VectorIterator<vector<int> > iter(v);
        
        CHECK(iter.hasMoreElements());
        CHECK_EQUAL(0, iter.getNext());

        CHECK(iter.hasMoreElements());
        CHECK_EQUAL(1, iter.getNext());

        CHECK(iter.hasMoreElements());
        CHECK_EQUAL(2, iter.getNext());

        CHECK(!iter.hasMoreElements());
    }


    TEST(PartialIteration)
    {
        vector<int> v;
        
        v.push_back(0);
        v.push_back(1);
        v.push_back(2);

        vector<int>::iterator start = v.begin();
        ++start;

        VectorIterator<vector<int> > iter(start, v.end());
        
        CHECK(iter.hasMoreElements());
        CHECK_EQUAL(1, iter.getNext());

        CHECK(iter.hasMoreElements());
        CHECK_EQUAL(2, iter.getNext());

        CHECK(!iter.hasMoreElements());
    }


    TEST(FullConstantIteration)
    {
        vector<int> v;
        
        v.push_back(0);
        v.push_back(1);
        v.push_back(2);

        ConstVectorIterator<vector<int> > iter(v);
        
        CHECK(iter.hasMoreElements());
        CHECK_EQUAL(0, iter.getNext());

        CHECK(iter.hasMoreElements());
        CHECK_EQUAL(1, iter.getNext());

        CHECK(iter.hasMoreElements());
        CHECK_EQUAL(2, iter.getNext());

        CHECK(!iter.hasMoreElements());
    }


    TEST(PartialConstantIteration)
    {
        vector<int> v;
        
        v.push_back(0);
        v.push_back(1);
        v.push_back(2);

        vector<int>::const_iterator start = v.begin();
        ++start;

        ConstVectorIterator<vector<int> > iter(start, v.end());
        
        CHECK(iter.hasMoreElements());
        CHECK_EQUAL(1, iter.getNext());

        CHECK(iter.hasMoreElements());
        CHECK_EQUAL(2, iter.getNext());

        CHECK(!iter.hasMoreElements());
    }


    TEST(PeekDoesNotAdvanceCurrentElement)
    {
        vector<int> v;
        
        v.push_back(0);
        v.push_back(1);
        v.push_back(2);

        VectorIterator<vector<int> > iter(v);
        
        CHECK(iter.hasMoreElements());
        CHECK_EQUAL(0, iter.peekNext());

        CHECK(iter.hasMoreElements());
        CHECK_EQUAL(0, iter.getNext());
    }


    TEST(MoveNext)
    {
        vector<int> v;
        
        v.push_back(0);
        v.push_back(1);
        v.push_back(2);

        VectorIterator<vector<int> > iter(v);
        
        CHECK(iter.hasMoreElements());
        
        iter.moveNext();
        
        CHECK(iter.hasMoreElements());
        CHECK_EQUAL(1, iter.getNext());
    }


    TEST(PeekNextPtr)
    {
        vector<int> v;
        
        v.push_back(0);
        v.push_back(1);
        v.push_back(2);

        VectorIterator<vector<int> > iter(v);
        
        int* p = iter.peekNextPtr();
        CHECK_EQUAL(0, *p);
        CHECK(iter.hasMoreElements());
    
        *p = 4;
    
        CHECK_EQUAL(4, iter.getNext());
    }
}


SUITE(MapIteratorTests)
{
    TEST(FullIteration)
    {
        map<string, int> m;
        
        m["first"] = 0;
        m["second"] = 1;
        m["third"] = 2;

        MapIterator<map<string, int> > iter(m);
        
        CHECK(iter.hasMoreElements());
        
        while (iter.hasMoreElements())
        {
            string key = iter.peekNextKey();
            if (key == "first")
                CHECK_EQUAL(0, iter.getNext());
            else if (key == "second")
                CHECK_EQUAL(1, iter.getNext());
            else if (key == "third")
                CHECK_EQUAL(2, iter.getNext());
            else
                CHECK(false && "Unknown key");
        }

        CHECK(!iter.hasMoreElements());
    }


    TEST(FullConstantIteration)
    {
        map<string, int> m;
        
        m["first"] = 0;
        m["second"] = 1;
        m["third"] = 2;

        ConstMapIterator<map<string, int> > iter(m);
        
        CHECK(iter.hasMoreElements());
        
        while (iter.hasMoreElements())
        {
            string key = iter.peekNextKey();
            if (key == "first")
                CHECK_EQUAL(0, iter.getNext());
            else if (key == "second")
                CHECK_EQUAL(1, iter.getNext());
            else if (key == "third")
                CHECK_EQUAL(2, iter.getNext());
            else
                CHECK(false && "Unknown key");
        }

        CHECK(!iter.hasMoreElements());
    }


    TEST(PeekDoesNotAdvanceCurrentElement)
    {
        map<string, int> m;
        
        m["first"] = 0;
        m["second"] = 1;
        m["third"] = 2;

        MapIterator<map<string, int> > iter(m);
        
        int v = iter.peekNextValue();
        
        CHECK(iter.hasMoreElements());
        CHECK_EQUAL(v, iter.getNext());
    }


    TEST(MoveNext)
    {
        map<string, int> m;
        
        m["first"] = 0;
        m["second"] = 1;
        m["third"] = 2;

        MapIterator<map<string, int> > iter(m);
        
        CHECK(iter.hasMoreElements());
        iter.moveNext();
        CHECK(iter.hasMoreElements());
        iter.moveNext();
        CHECK(iter.hasMoreElements());
        iter.moveNext();
        CHECK(!iter.hasMoreElements());
    }


    TEST(PeekNextPtr)
    {
        map<string, int> m;
        
        m["first"] = 0;
        m["second"] = 1;
        m["third"] = 2;

        MapIterator<map<string, int> > iter(m);
        
        string key = iter.peekNextKey();
        int* p = iter.peekNextValuePtr();
        
        CHECK(iter.hasMoreElements());
    
        *p = 4;
    
        CHECK_EQUAL(4, m[key]);
    }
}
