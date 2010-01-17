/** @file	ListIterator.h
	@author	Philip Abbet

	Declaration of the iterators classes
    
    @note   This is based on the corresponding class from
            <a href="http://www.ogre3d.org/">Ogre3D</a>.
*/

#ifndef _ATHENA_UTILS_LISTITERATOR_H_
#define _ATHENA_UTILS_LISTITERATOR_H_

#include <Athena-Core/Prerequisites.h>


namespace Athena {
namespace Utils {

//---------------------------------------------------------------------------------------
/// @brief  Base functionality for the iterators over a container of values
///
/// @param  T               A container like vector, list, ...
/// @param  IteratorType    T::iterator or T::const_iterator
///
/// @remark Have a look at VectorIterator and ConstVectorIterator for a concrete usage
///
/// @remark This class is here just to allow clients to iterate over an internal list of
///         a class without having to have access to the list itself. No updates are
///         allowed through this interface, it is purely for iterating and reading.
///
/// @remark Note that like STL iterators, these iterators are only valid whilst no
///         updates are made to the underlying collection. You should not attempt to use
///         this iterator if a change is made to the collection. In fact, treat this
///         iterator as a transient object, do NOT store it and try to use it repeatedly.
///    
/// @note   This is based on the corresponding class from
///         <a href="http://www.ogre3d.org/">Ogre3D</a>.
//---------------------------------------------------------------------------------------
template <typename T, typename IteratorType>
class ValuesIterator
{
    //_____ Internal types __________
public:
	typedef typename T::value_type   ValueType;      /// Type returned by peekNext
	typedef typename T::value_type*  PointerType;    /// Type returned by peekNextPtr


    //_____ Construction / Destruction __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Constructor
	///
	/// @param  start   Where to start the iteration
	/// @param  end     Where to end the iteration
	//-----------------------------------------------------------------------------------
	ValuesIterator(IteratorType start, IteratorType end)
	: mCurrent(start), mEnd(end)
	{
	}


    //_____ Methods __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Indicates if there are more items in the collection
	//-----------------------------------------------------------------------------------
	bool hasMoreElements() const
	{
		return mCurrent != mEnd;
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the next (=current) element in the collection, without advancing
	///         to the next afterwards
	//-----------------------------------------------------------------------------------
	ValueType peekNext() const
	{
		return *mCurrent;
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Returns a pointer to the next (=current) element in the collection,
	///         without advancing to the next afterwards
	//-----------------------------------------------------------------------------------
	PointerType peekNextPtr() const
	{
		return &(*mCurrent);
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the next (=current) value element in the collection, and advances
	///         to the next
	//-----------------------------------------------------------------------------------
	ValueType getNext() 
	{
		return *(this->mCurrent++);
	}	

	//-----------------------------------------------------------------------------------
	/// @brief	Moves the iterator of one element
	//-----------------------------------------------------------------------------------
	void moveNext()
	{
		++mCurrent;
	}


    //_____ Attributes __________
protected:
	IteratorType mCurrent;
	IteratorType mEnd;
};


//---------------------------------------------------------------------------------------
/// @brief  Iterator over a container of values (vector, list)
///
/// @param  T   A container like vector or list
///    
/// @note   This is based on the corresponding class from
///         <a href="http://www.ogre3d.org/">Ogre3D</a>.
//---------------------------------------------------------------------------------------
template <typename T>
class VectorIterator : public ValuesIterator<T, typename T::iterator>
{
    //_____ Construction / Destruction __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Constructor
	///
	/// @param  start   Where to start the iteration
	/// @param  end     Where to end the iteration
	//-----------------------------------------------------------------------------------
	VectorIterator(typename T::iterator start, typename T::iterator end)
	: ValuesIterator<T, typename T::iterator>(start, end)
	{
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Constructor
	///
	/// @param  container   The container over which to iterate
	//-----------------------------------------------------------------------------------
	explicit VectorIterator(T& container)
	: ValuesIterator<T, typename T::iterator>(container.begin(), container.end())
	{
	}
};


//---------------------------------------------------------------------------------------
/// @brief  Constant iterator over a container of values (vector, list)
///
/// @param  T   A container like vector or list
///    
/// @note   This is based on the corresponding class from
///         <a href="http://www.ogre3d.org/">Ogre3D</a>.
//---------------------------------------------------------------------------------------
template <typename T>
class ConstVectorIterator : public ValuesIterator<T, typename T::const_iterator>
{
    //_____ Construction / Destruction __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Constructor
	///
	/// @param  start   Where to start the iteration
	/// @param  end     Where to end the iteration
	//-----------------------------------------------------------------------------------
	ConstVectorIterator(typename T::const_iterator start, typename T::const_iterator end)
	: ValuesIterator<T, typename T::const_iterator>(start, end)
	{
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Constructor
	///
	/// @param  container   The container over which to iterate
	//-----------------------------------------------------------------------------------
	explicit ConstVectorIterator(const T& container)
	: ValuesIterator<T, typename T::const_iterator>(container.begin(), container.end())
	{
	}
};



//---------------------------------------------------------------------------------------
/// @brief  Base functionality for the iterators over a key-value container
///
/// @param  T               A key-value container like map, set, ...
/// @param  IteratorType    T::iterator or T::const_iterator
///
/// @remark Have a look at MapIterator and ConstMapIterator for a concrete usage
///    
/// @remark This class is here just to allow clients to iterate over an internal list of
///         a class without having to have access to the list itself. No updates are
///         allowed through this interface, it is purely for iterating and reading.
///
/// @remark Note that like STL iterators, these iterators are only valid whilst no
///         updates are made to the underlying collection. You should not attempt to use
///         this iterator if a change is made to the collection. In fact, treat this
///         iterator as a transient object, do NOT store it and try to use it repeatedly.
///    
/// @note   This is based on the corresponding class from
///         <a href="http://www.ogre3d.org/">Ogre3D</a>.
//---------------------------------------------------------------------------------------
template <typename T, typename IteratorType>
class KeyValueIterator
{
    //_____ Internal types __________
public:
	typedef typename T::mapped_type  ValueType;      /// Type returned by peekNextValue
	typedef typename T::mapped_type* PointerType;    /// Type returned by peekNextValuePtr
	typedef typename T::key_type     KeyType;        /// Type returned by peekNextKey


    //_____ Construction / Destruction __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Constructor
	///
	/// @param  start   Where to start the iteration
	/// @param  end     Where to end the iteration
	//-----------------------------------------------------------------------------------
	KeyValueIterator(IteratorType start, IteratorType end)
	: mCurrent(start), mEnd(end)
	{
	}


    //_____ Methods __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Indicates if there are more items in the collection
	//-----------------------------------------------------------------------------------
	bool hasMoreElements() const
	{
		return mCurrent != mEnd;
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the next (=current) key element in the collection, without
	///         advancing to the next afterwards
	//-----------------------------------------------------------------------------------
    KeyType peekNextKey() const
    {
        return this->mCurrent->first;
    }

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the next (=current) value element in the collection, without
	///         advancing to the next afterwards
	//-----------------------------------------------------------------------------------
	ValueType peekNextValue() const
	{
		return this->mCurrent->second;
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Returns a pointer to the next (=current) value element in the collection,
	///         without advancing to the next afterwards
	//-----------------------------------------------------------------------------------
	const PointerType peekNextValuePtr() const
	{
		return &(this->mCurrent->second);
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Returns the next (=current) value element in the collection, and advances
	///         to the next afterwards
	//-----------------------------------------------------------------------------------
    ValueType getNext()
    {
        return ((this->mCurrent++)->second);
    }
	
	//-----------------------------------------------------------------------------------
	/// @brief	Moves the iterator of one element
	//-----------------------------------------------------------------------------------
	void moveNext()
	{
		++mCurrent;
	}


    //_____ Attributes __________
protected:
	IteratorType mCurrent;
	IteratorType mEnd;
};


//---------------------------------------------------------------------------------------
/// @brief  Iterator over a key-value container (map, set)
///
/// @param  T   A container like map or set
///    
/// @note   This is based on the corresponding class from
///         <a href="http://www.ogre3d.org/">Ogre3D</a>.
//---------------------------------------------------------------------------------------
template <typename T>
class MapIterator : public KeyValueIterator<T, typename T::iterator>
{
    //_____ Construction / Destruction __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Constructor
	///
	/// @param  start   Where to start the iteration
	/// @param  end     Where to end the iteration
	//-----------------------------------------------------------------------------------
	MapIterator(typename T::iterator start, typename T::iterator end)
	: KeyValueIterator<T, typename T::iterator>(start, end)
	{
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Constructor
	///
	/// @param  container   The container over which to iterate
	//-----------------------------------------------------------------------------------
	explicit MapIterator(T& container)
	: KeyValueIterator<T, typename T::iterator>(container.begin(), container.end())
	{
	}
};


//---------------------------------------------------------------------------------------
/// @brief  Constant iterator over a key-value container (map, set)
///
/// @param  T   A container like map or set
///    
/// @note   This is based on the corresponding class from
///         <a href="http://www.ogre3d.org/">Ogre3D</a>.
//---------------------------------------------------------------------------------------
template <typename T>
class ConstMapIterator : public KeyValueIterator<T, typename T::const_iterator>
{
    //_____ Construction / Destruction __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Constructor
	///
	/// @param  start   Where to start the iteration
	/// @param  end     Where to end the iteration
	//-----------------------------------------------------------------------------------
	ConstMapIterator(typename T::const_iterator start, typename T::const_iterator end)
	: KeyValueIterator<T, typename T::const_iterator>(start, end)
	{
	}

	//-----------------------------------------------------------------------------------
	/// @brief	Constructor
	///
	/// @param  container   The container over which to iterate
	//-----------------------------------------------------------------------------------
	explicit ConstMapIterator(const T& container)
    : KeyValueIterator<T, typename T::const_iterator>(container.begin(), container.end())
	{
	}
};

}
}

#endif
