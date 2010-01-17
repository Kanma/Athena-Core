/**	@file	Variant.h
	@author	Philip Abbet

	Declaration of the class 'Athena::Utils::Variant'
*/

#ifndef _ATHENA_UTILS_VARIANT_H_
#define _ATHENA_UTILS_VARIANT_H_

#include <Athena-Core/Prerequisites.h>
#include <Athena-Core/Utils/Iterators.h>

namespace Athena {
namespace Utils {

//-----------------------------------------------------------------------------------
/// @brief	Acts like a union of several data types
//-----------------------------------------------------------------------------------
class ATHENA_CORE_SYMBOL Variant
{
	//_____ Internal types __________
public:
	//-----------------------------------------------------------------------------------
	/// @brief	Enumerates the possible type of data contained in a Variant object
	//-----------------------------------------------------------------------------------
	enum tType
	{
		NONE,
		STRING,
		INTEGER,
		SHORT,
		CHAR,
		UNSIGNED_INTEGER,
		UNSIGNED_SHORT,
		UNSIGNED_CHAR,
		FLOAT,
		DOUBLE,
		BOOLEAN,
		VECTOR3,
		QUATERNION,
		COLOR,
		RADIAN,
		DEGREE,
		STRUCT,
	};

	typedef std::map<std::string, Variant*>	tFieldsMap;
	typedef MapIterator<tFieldsMap>		    tFieldsIterator;


	//_____ Construction / Destruction __________
public:
	Variant();
	Variant(tType type);
	Variant(int value);
	Variant(short value);
	Variant(char value);
	Variant(unsigned int value);
	Variant(unsigned short value);
	Variant(unsigned char value);
	Variant(float value);
	Variant(double value);
	Variant(bool value);
	Variant(const std::string& strValue);
	Variant(const char* strValue);
	Variant(const Math::Vector3& value);
	Variant(const Math::Quaternion& value);
	Variant(const Math::Color& value);
	Variant(const Math::Radian& value);
	Variant(const Math::Degree& value);
	Variant(const Variant& value);
	~Variant();

	void operator=(const Variant& value);

protected:
	void clear();


	//_____ Management of the type __________
public:
	inline tType getType() const
	{
		return m_type;
	}

	inline bool hasType(tType type) const
	{
		return (m_type == type);
	}


	//_____ Retrieval of the value __________
public:
	inline bool isNull() const
	{
		return m_bNull;
	}


	int					toInt() const;
	short				toShort() const;
	char				toChar() const;
	unsigned int		toUInt() const;
	unsigned short		toUShort() const;
	unsigned char		toUChar() const;
	float				toFloat() const;
	double				toDouble() const;
	bool				toBool() const;
	std::string		    toString() const;
	Math::Vector3		toVector3() const;
	Math::Quaternion	toQuaternion() const;
	Math::Color	        toColor() const;
	Math::Radian		toRadian() const;
	Math::Degree		toDegree() const;


	//_____ Struct-related methods __________
public:
	void setField(const std::string& strName, Variant* pValue);
	Variant* getField(const std::string& strName);
	tFieldsIterator getFieldsIterator();


	//_____ Type conversion __________
public:
	bool convertTo(tType type);


	//_____ Attributes __________
private:
	tType	m_type;
	bool	m_bNull;
	
	union
	{
		int				_int;
		short			_short;
		char			_char;
		unsigned int	_uint;
		unsigned short	_ushort;
		unsigned char	_uchar;
		float			_float;
		double			_double;
		bool			_bool;
		void*			_others;
	} m_value;
};

}
}

#endif
