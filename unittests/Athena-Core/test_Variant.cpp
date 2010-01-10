#include <UnitTest++.h>
#include <Athena-Core/Variant.h>
#include <Athena-Math/Vector3.h>
#include <Athena-Math/Quaternion.h>
#include <Athena-Math/Color.h>

using namespace Athena::Utils;
using namespace Athena::Math;
using namespace std;


#define DECLARE_TEST_VARIANT_INITIALIZATION(SRC_TYPE, SRC_TYPE_ID, SRC_VALUE, GETTER)	\
	TEST(VariantContains##SRC_TYPE_ID)													\
	{																					\
		SRC_TYPE val = SRC_VALUE;														\
		Variant v(val);																	\
		CHECK_EQUAL(Variant::SRC_TYPE_ID, v.getType());									\
		CHECK(v.hasType(Variant::SRC_TYPE_ID));											\
		CHECK(!v.isNull());																\
		CHECK(SRC_VALUE == v.GETTER());													\
	}


SUITE(VariantInitializationSuite)
{
	TEST(VariantNotInitialized)
	{
		Variant v;
		CHECK_EQUAL(Variant::NONE, v.getType());
		CHECK(v.hasType(Variant::NONE));
		CHECK(v.isNull());
	}


	DECLARE_TEST_VARIANT_INITIALIZATION(string,			STRING,				"test",	toString)
	DECLARE_TEST_VARIANT_INITIALIZATION(int,			INTEGER,			10,		toInt)
	DECLARE_TEST_VARIANT_INITIALIZATION(short,			SHORT,				10,		toShort)
	DECLARE_TEST_VARIANT_INITIALIZATION(char,			CHAR,				10,		toChar)
	DECLARE_TEST_VARIANT_INITIALIZATION(unsigned int,	UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VARIANT_INITIALIZATION(unsigned short,	UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VARIANT_INITIALIZATION(unsigned char,	UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VARIANT_INITIALIZATION(float,			FLOAT,				10.0f,	toFloat)
	DECLARE_TEST_VARIANT_INITIALIZATION(double,			DOUBLE,				10.0,	toDouble)
	DECLARE_TEST_VARIANT_INITIALIZATION(bool,			BOOLEAN,			true,	toBool)
	
	DECLARE_TEST_VARIANT_INITIALIZATION(Vector3,		VECTOR3,	Vector3(10.0f, 20.0f, 30.0f),			toVector3)
	DECLARE_TEST_VARIANT_INITIALIZATION(Quaternion,		QUATERNION,	Quaternion(10.0f, 20.0f, 30.0f, 40.0f),	toQuaternion)
	DECLARE_TEST_VARIANT_INITIALIZATION(Color,	        COLOR,	    Color(10.0f, 20.0f, 30.0f, 40.0f),	    toColor)

	TEST(VariantContainsRadian)
	{
		Radian val(10.0f);
		Variant v(val);
		CHECK_EQUAL(Variant::RADIAN, v.getType());
		CHECK(v.hasType(Variant::RADIAN));
		CHECK(!v.isNull());
		CHECK_CLOSE(val.valueRadians(), v.toRadian().valueRadians(), 0.0001f);
	}

	TEST(VariantContainsDegree)
	{
		Degree val(10.0f);
		Variant v(val);
		CHECK_EQUAL(Variant::DEGREE, v.getType());
		CHECK(v.hasType(Variant::DEGREE));
		CHECK(!v.isNull());
		CHECK_CLOSE(val.valueDegrees(), v.toDegree().valueDegrees(), 0.0001f);
	}
}

#undef DECLARE_TEST_VARIANT_INITIALIZATION



#define DECLARE_TEST_NULL_VARIANT_INITALIZATION(TYPE_ID, DEFAULT_VALUE, GETTER)			\
	TEST(NullVariantInitializedAs##TYPE_ID)												\
	{																					\
		Variant v(Variant::TYPE_ID);													\
		CHECK_EQUAL(Variant::TYPE_ID, v.getType());										\
		CHECK(v.hasType(Variant::TYPE_ID));												\
		CHECK(v.isNull());																\
		CHECK(DEFAULT_VALUE == v.GETTER());												\
	}


SUITE(NullVariantSuite)
{
	DECLARE_TEST_NULL_VARIANT_INITALIZATION(STRING,				"",					toString)
	DECLARE_TEST_NULL_VARIANT_INITALIZATION(INTEGER,			0,					toInt)
	DECLARE_TEST_NULL_VARIANT_INITALIZATION(SHORT,				0,					toShort)
	DECLARE_TEST_NULL_VARIANT_INITALIZATION(CHAR,				0,					toChar)
	DECLARE_TEST_NULL_VARIANT_INITALIZATION(UNSIGNED_INTEGER,	0,					toUInt)
	DECLARE_TEST_NULL_VARIANT_INITALIZATION(UNSIGNED_SHORT,		0,					toUShort)
	DECLARE_TEST_NULL_VARIANT_INITALIZATION(UNSIGNED_CHAR,		0,					toUChar)
	DECLARE_TEST_NULL_VARIANT_INITALIZATION(FLOAT,				0.0f,				toFloat)
	DECLARE_TEST_NULL_VARIANT_INITALIZATION(DOUBLE,				0.0,				toDouble)
	DECLARE_TEST_NULL_VARIANT_INITALIZATION(BOOLEAN,			false,				toBool)
	DECLARE_TEST_NULL_VARIANT_INITALIZATION(VECTOR3,			Vector3::ZERO,		toVector3)
	DECLARE_TEST_NULL_VARIANT_INITALIZATION(QUATERNION,			Quaternion::ZERO,	toQuaternion)
	DECLARE_TEST_NULL_VARIANT_INITALIZATION(COLOR,				Color::ZERO,	    toColor)

	TEST(NullVariantInitializedAsRadian)
	{
		Variant v(Variant::RADIAN);
		CHECK_EQUAL(Variant::RADIAN, v.getType());
		CHECK(v.hasType(Variant::RADIAN));
		CHECK(v.isNull());
		CHECK_CLOSE(0.0f, v.toRadian().valueRadians(), 0.0001f);
	}

	TEST(NullVariantInitializedAsDegree)
	{
		Variant v(Variant::DEGREE);
		CHECK_EQUAL(Variant::DEGREE, v.getType());
		CHECK(v.hasType(Variant::DEGREE));
		CHECK(v.isNull());
		CHECK_CLOSE(0.0f, v.toDegree().valueDegrees(), 0.0001f);
	}
}

#undef DECLARE_TEST_NULL_VARIANT_INITALIZATION



#define DECLARE_TEST_VARIANT_COPY(TYPE, TYPE_ID, VALUE, GETTER)							\
	TEST(VariantCopy##TYPE_ID)															\
	{																					\
		Variant v;																		\
		{																				\
			TYPE val = VALUE;															\
			Variant v2(val);															\
			v = v2;																		\
		}																				\
		CHECK_EQUAL(Variant::TYPE_ID, v.getType());										\
		CHECK(v.hasType(Variant::TYPE_ID));												\
		CHECK(!v.isNull());																\
		CHECK(VALUE == v.GETTER());														\
	}


SUITE(VariantCopySuite)
{
	DECLARE_TEST_VARIANT_COPY(string,			STRING,				"test",	toString)
	DECLARE_TEST_VARIANT_COPY(int,				INTEGER,			10,		toInt)
	DECLARE_TEST_VARIANT_COPY(short,			SHORT,				10,		toShort)
	DECLARE_TEST_VARIANT_COPY(char,				CHAR,				10,		toChar)
	DECLARE_TEST_VARIANT_COPY(unsigned int,		UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VARIANT_COPY(unsigned short,	UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VARIANT_COPY(unsigned char,	UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VARIANT_COPY(float,			FLOAT,				10.0f,	toFloat)
	DECLARE_TEST_VARIANT_COPY(double,			DOUBLE,				10.0,	toDouble)
	DECLARE_TEST_VARIANT_COPY(bool,				BOOLEAN,			true,	toBool)
	
	DECLARE_TEST_VARIANT_COPY(Vector3,		VECTOR3,	Vector3(10.0f, 20.0f, 30.0f),			toVector3)
	DECLARE_TEST_VARIANT_COPY(Quaternion,	QUATERNION,	Quaternion(10.0f, 20.0f, 30.0f, 40.0f),	toQuaternion)
	DECLARE_TEST_VARIANT_COPY(Color,	    COLOR,		Color(10.0f, 20.0f, 30.0f, 40.0f),	    toColor)

	TEST(VariantCopyRadian)
	{
		Variant v;
		{
			Radian val(10.0f);
			Variant v2(val);
			v = v2;
		}
		CHECK_EQUAL(Variant::RADIAN, v.getType());
		CHECK(v.hasType(Variant::RADIAN));
		CHECK(!v.isNull());
		CHECK_CLOSE(10.0f, v.toRadian().valueRadians(), 0.0001f);
	}

	TEST(VariantCopyDegree)
	{
		Variant v;
		{
			Degree val(10.0f);
			Variant v2(val);
			v = v2;
		}
		CHECK_EQUAL(Variant::DEGREE, v.getType());
		CHECK(v.hasType(Variant::DEGREE));
		CHECK(!v.isNull());
		CHECK_CLOSE(10.0f,  v.toDegree().valueDegrees(), 0.0001f);
	}
}

#undef DECLARE_TEST_VARIANT_COPY



SUITE(VariantStructSuite)
{
	TEST(VariantStructCreation)
	{
		Variant v(Variant::STRUCT);
		CHECK_EQUAL(Variant::STRUCT, v.getType());
		CHECK(v.hasType(Variant::STRUCT));
		CHECK(!v.isNull());

		Variant::tFieldsIterator iter = v.getFieldsIterator();
		CHECK(!iter.hasMoreElements());
	}


	TEST(VariantStructWithFields)
	{
		Variant v(Variant::STRUCT);
		CHECK_EQUAL(Variant::STRUCT, v.getType());
		CHECK(v.hasType(Variant::STRUCT));
		CHECK(!v.isNull());

		v.setField("string", new Variant("test"));
		v.setField("bool", new Variant(true));
		v.setField("int", new Variant(25));

		Variant::tFieldsIterator iter = v.getFieldsIterator();
		CHECK(iter.hasMoreElements());

		CHECK_EQUAL("test", v.getField("string")->toString());
		CHECK_EQUAL(true, v.getField("bool")->toBool());
		CHECK_EQUAL(25, v.getField("int")->toInt());
	}


	TEST(VariantStructFieldsIteration)
	{
		Variant v(Variant::STRUCT);
		CHECK_EQUAL(Variant::STRUCT, v.getType());
		CHECK(v.hasType(Variant::STRUCT));
		CHECK(!v.isNull());

		v.setField("string", new Variant("test"));
		v.setField("bool", new Variant(true));
		v.setField("int", new Variant(25));

		Variant::tFieldsIterator iter = v.getFieldsIterator();
		CHECK(iter.hasMoreElements());

		unsigned int i = 0;
		while (iter.hasMoreElements())
		{
			if (iter.peekNextKey() == "string")
				CHECK_EQUAL("test", iter.getNext()->toString());
			else if (iter.peekNextKey() == "bool")
				CHECK_EQUAL(true, iter.getNext()->toBool());
			else if (iter.peekNextKey() == "int")
				CHECK_EQUAL(25, iter.getNext()->toInt());
		
			++i;
		}

		CHECK_EQUAL(3, i);
	}


	TEST(VariantStructCopy)
	{
		Variant v;
		{
			Variant v2(Variant::STRUCT);
			v2.setField("string", new Variant("test"));
			v2.setField("bool", new Variant(true));
			v2.setField("int", new Variant(25));

			v = v2;
		}

		CHECK_EQUAL(Variant::STRUCT, v.getType());
		CHECK(v.hasType(Variant::STRUCT));
		CHECK(!v.isNull());

		Variant::tFieldsIterator iter = v.getFieldsIterator();
		CHECK(iter.hasMoreElements());

		CHECK_EQUAL("test", v.getField("string")->toString());
		CHECK_EQUAL(true, v.getField("bool")->toBool());
		CHECK_EQUAL(25, v.getField("int")->toInt());
	}
}


#define DECLARE_TEST_VALID_CONVERSION_N(N, SRC_TYPE, SRC_TYPE_ID, SRC_VALUE, DST_TYPE_ID, DST_VALUE, GETTER)	\
	TEST(ValidConversionFrom##SRC_TYPE_ID##To##DST_TYPE_ID##_##N)							\
	{																						\
		SRC_TYPE val = SRC_VALUE;															\
		Variant v(val);												    					\
																							\
		CHECK(v.hasType(Variant::SRC_TYPE_ID));					    						\
																							\
		CHECK(v.convertTo(Variant::DST_TYPE_ID));											\
																							\
		CHECK(v.hasType(Variant::DST_TYPE_ID));				    							\
																							\
		CHECK(DST_VALUE == v.GETTER());					    								\
	}


#define DECLARE_TEST_VALID_CONVERSION(SRC_TYPE, SRC_TYPE_ID, SRC_VALUE, DST_TYPE_ID, DST_VALUE, GETTER)	\
	DECLARE_TEST_VALID_CONVERSION_N(0, SRC_TYPE, SRC_TYPE_ID, SRC_VALUE, DST_TYPE_ID, DST_VALUE, GETTER)


#define DECLARE_TEST_INVALID_CONVERSION(SRC_TYPE, SRC_TYPE_ID, SRC_VALUE, DST_TYPE_ID)		\
	TEST(InvalidConversionFrom##SRC_TYPE_ID##To##DST_TYPE_ID)								\
	{																						\
		SRC_TYPE val = SRC_VALUE;															\
		Variant v(val);													    				\
																							\
		CHECK(v.hasType(Variant::SRC_TYPE_ID));								    			\
																							\
		CHECK(!v.convertTo(Variant::DST_TYPE_ID));											\
																							\
		CHECK(v.hasType(Variant::SRC_TYPE_ID));									    		\
	}


SUITE(VariantExplicitConversionsSuite)
{
	TEST(ConversionFromNullVariantToNullVariant)
	{
		Variant v(Variant::STRING);

		CHECK(v.hasType(Variant::STRING));
		CHECK(v.isNull());

		CHECK(v.convertTo(Variant::INTEGER));

		CHECK(v.hasType(Variant::INTEGER));
		CHECK(v.isNull());
	}


	// Valid conversion: from string to ...
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "test",	STRING,				"test",	toString)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "10",		INTEGER,			10,		toInt)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "10",		SHORT,				10,		toShort)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "10",		CHAR,				10,		toChar)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "10",		UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "10",		UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "10",		UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "10.0",	FLOAT,				10.0f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "10.0",	DOUBLE,				10.0,	toDouble)

	DECLARE_TEST_VALID_CONVERSION_N(0, string, STRING, "True",	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(1, string, STRING, "true",	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(2, string, STRING, "TRUE",	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(3, string, STRING, "False",	BOOLEAN, false,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(4, string, STRING, "false",	BOOLEAN, false,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(5, string, STRING, "FALSE",	BOOLEAN, false,	toBool)

	DECLARE_TEST_VALID_CONVERSION(string, STRING, "(10.0, 20.0, 30.0)",	VECTOR3, Vector3(10.0f, 20.0f, 30.0f), toVector3)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "(10.0, 20.0, 30.0, 40.0)", QUATERNION, Quaternion(10.0f, 20.0f, 30.0f, 40.0f), toQuaternion)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "(10.0, 20.0, 30.0, 40.0)", COLOR, Color(10.0f, 20.0f, 30.0f, 40.0f), toColor)

	TEST(ValidConversionFromStringToRadian)
	{
		Variant v("10.5");
		CHECK(v.hasType(Variant::STRING));
		CHECK(v.convertTo(Variant::RADIAN));
		CHECK(v.hasType(Variant::RADIAN));
		CHECK_CLOSE(10.5, v.toRadian().valueRadians(), 0.0001f);
	}

	TEST(ValidConversionFromStringToDegree)
	{
		Variant v("10.5");
		CHECK(v.hasType(Variant::STRING));
		CHECK(v.convertTo(Variant::DEGREE));
		CHECK(v.hasType(Variant::DEGREE));
		CHECK_CLOSE(10.5, v.toDegree().valueDegrees(), 0.0001f);
	}

	// Invalid conversion: from string to ...
	DECLARE_TEST_INVALID_CONVERSION(string, STRING, "test", INTEGER)
	DECLARE_TEST_INVALID_CONVERSION(string, STRING, "test", SHORT)
	DECLARE_TEST_INVALID_CONVERSION(string, STRING, "test", CHAR)
	DECLARE_TEST_INVALID_CONVERSION(string, STRING, "test", UNSIGNED_INTEGER)
	DECLARE_TEST_INVALID_CONVERSION(string, STRING, "test", UNSIGNED_SHORT)	
	DECLARE_TEST_INVALID_CONVERSION(string, STRING, "test", UNSIGNED_CHAR)	
	DECLARE_TEST_INVALID_CONVERSION(string, STRING, "test", FLOAT)	
	DECLARE_TEST_INVALID_CONVERSION(string, STRING, "test", DOUBLE)	
	DECLARE_TEST_INVALID_CONVERSION(string, STRING, "test", BOOLEAN)	

	// From integer to ...
	DECLARE_TEST_VALID_CONVERSION(int, INTEGER, 10,	STRING,				"10",	toString)
	DECLARE_TEST_VALID_CONVERSION(int, INTEGER, 10,	INTEGER,			10,		toInt)
	DECLARE_TEST_VALID_CONVERSION(int, INTEGER, 10,	SHORT,				10,		toShort)
	DECLARE_TEST_VALID_CONVERSION(int, INTEGER, 10,	CHAR,				10,		toChar)
	DECLARE_TEST_VALID_CONVERSION(int, INTEGER, 10,	UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VALID_CONVERSION(int, INTEGER, 10,	UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VALID_CONVERSION(int, INTEGER, 10,	UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VALID_CONVERSION(int, INTEGER, 10,	FLOAT,				10.0f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION(int, INTEGER, 10,	DOUBLE,				10.0,	toDouble)
	
	DECLARE_TEST_VALID_CONVERSION_N(0, int, INTEGER, 1,	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(1, int, INTEGER, 0,	BOOLEAN, false,	toBool)

	// From short to ...
	DECLARE_TEST_VALID_CONVERSION(short, SHORT, 10,	STRING,				"10",	toString)
	DECLARE_TEST_VALID_CONVERSION(short, SHORT, 10,	INTEGER,			10,		toInt)
	DECLARE_TEST_VALID_CONVERSION(short, SHORT, 10,	SHORT,				10,		toShort)
	DECLARE_TEST_VALID_CONVERSION(short, SHORT, 10,	CHAR,				10,		toChar)
	DECLARE_TEST_VALID_CONVERSION(short, SHORT, 10,	UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VALID_CONVERSION(short, SHORT, 10,	UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VALID_CONVERSION(short, SHORT, 10,	UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VALID_CONVERSION(short, SHORT, 10,	FLOAT,				10.0f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION(short, SHORT, 10,	DOUBLE,				10.0,	toDouble)

	DECLARE_TEST_VALID_CONVERSION_N(0, short, SHORT, 1,	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(1, short, SHORT, 0,	BOOLEAN, false,	toBool)

	// From char to ...
	DECLARE_TEST_VALID_CONVERSION(char, CHAR, 10,	STRING,				"10",	toString)
	DECLARE_TEST_VALID_CONVERSION(char, CHAR, 10,	INTEGER,			10,		toInt)
	DECLARE_TEST_VALID_CONVERSION(char, CHAR, 10,	SHORT,				10,		toShort)
	DECLARE_TEST_VALID_CONVERSION(char, CHAR, 10,	CHAR,				10,		toChar)
	DECLARE_TEST_VALID_CONVERSION(char, CHAR, 10,	UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VALID_CONVERSION(char, CHAR, 10,	UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VALID_CONVERSION(char, CHAR, 10,	UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VALID_CONVERSION(char, CHAR, 10,	FLOAT,				10.0f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION(char, CHAR, 10,	DOUBLE,				10.0,	toDouble)

	DECLARE_TEST_VALID_CONVERSION_N(0, char, CHAR, 1,	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(1, char, CHAR, 0,	BOOLEAN, false,	toBool)

	// From unsigned integer to ...
	DECLARE_TEST_VALID_CONVERSION(unsigned int, UNSIGNED_INTEGER, 10,	STRING,				"10",	toString)
	DECLARE_TEST_VALID_CONVERSION(unsigned int, UNSIGNED_INTEGER, 10,	INTEGER,			10,		toInt)
	DECLARE_TEST_VALID_CONVERSION(unsigned int, UNSIGNED_INTEGER, 10,	SHORT,				10,		toShort)
	DECLARE_TEST_VALID_CONVERSION(unsigned int, UNSIGNED_INTEGER, 10,	CHAR,				10,		toChar)
	DECLARE_TEST_VALID_CONVERSION(unsigned int, UNSIGNED_INTEGER, 10,	UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VALID_CONVERSION(unsigned int, UNSIGNED_INTEGER, 10,	UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VALID_CONVERSION(unsigned int, UNSIGNED_INTEGER, 10,	UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VALID_CONVERSION(unsigned int, UNSIGNED_INTEGER, 10,	FLOAT,				10.0f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION(unsigned int, UNSIGNED_INTEGER, 10,	DOUBLE,				10.0,	toDouble)

	DECLARE_TEST_VALID_CONVERSION_N(0, unsigned int, UNSIGNED_INTEGER, 1,	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(1, unsigned int, UNSIGNED_INTEGER, 0,	BOOLEAN, false,	toBool)

	// From unsigned short to ...
	DECLARE_TEST_VALID_CONVERSION(unsigned short, UNSIGNED_SHORT, 10,	STRING,				"10",	toString)
	DECLARE_TEST_VALID_CONVERSION(unsigned short, UNSIGNED_SHORT, 10,	INTEGER,			10,		toInt)
	DECLARE_TEST_VALID_CONVERSION(unsigned short, UNSIGNED_SHORT, 10,	SHORT,				10,		toShort)
	DECLARE_TEST_VALID_CONVERSION(unsigned short, UNSIGNED_SHORT, 10,	CHAR,				10,		toChar)
	DECLARE_TEST_VALID_CONVERSION(unsigned short, UNSIGNED_SHORT, 10,	UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VALID_CONVERSION(unsigned short, UNSIGNED_SHORT, 10,	UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VALID_CONVERSION(unsigned short, UNSIGNED_SHORT, 10,	UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VALID_CONVERSION(unsigned short, UNSIGNED_SHORT, 10,	FLOAT,				10.0f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION(unsigned short, UNSIGNED_SHORT, 10,	DOUBLE,				10.0,	toDouble)

	DECLARE_TEST_VALID_CONVERSION_N(0, unsigned short, UNSIGNED_SHORT, 1,	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(1, unsigned short, UNSIGNED_SHORT, 0,	BOOLEAN, false,	toBool)

	// From unsigned char to ...
	DECLARE_TEST_VALID_CONVERSION(unsigned char, UNSIGNED_CHAR, 10,	STRING,				"10",	toString)
	DECLARE_TEST_VALID_CONVERSION(unsigned char, UNSIGNED_CHAR, 10,	INTEGER,			10,		toInt)
	DECLARE_TEST_VALID_CONVERSION(unsigned char, UNSIGNED_CHAR, 10,	SHORT,				10,		toShort)
	DECLARE_TEST_VALID_CONVERSION(unsigned char, UNSIGNED_CHAR, 10,	CHAR,				10,		toChar)
	DECLARE_TEST_VALID_CONVERSION(unsigned char, UNSIGNED_CHAR, 10,	UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VALID_CONVERSION(unsigned char, UNSIGNED_CHAR, 10,	UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VALID_CONVERSION(unsigned char, UNSIGNED_CHAR, 10,	UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VALID_CONVERSION(unsigned char, UNSIGNED_CHAR, 10,	FLOAT,				10.0f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION(unsigned char, UNSIGNED_CHAR, 10,	DOUBLE,				10.0,	toDouble)

	DECLARE_TEST_VALID_CONVERSION_N(0, unsigned char, UNSIGNED_CHAR, 1,	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(1, unsigned char, UNSIGNED_CHAR, 0,	BOOLEAN, false,	toBool)

	// From float to ...
	DECLARE_TEST_VALID_CONVERSION(float, FLOAT, 10.5f,	STRING,				"10.5",	toString)
	DECLARE_TEST_VALID_CONVERSION(float, FLOAT, 10.5f,	INTEGER,			10,		toInt)
	DECLARE_TEST_VALID_CONVERSION(float, FLOAT, 10.5f,	SHORT,				10,		toShort)
	DECLARE_TEST_VALID_CONVERSION(float, FLOAT, 10.5f,	CHAR,				10,		toChar)
	DECLARE_TEST_VALID_CONVERSION(float, FLOAT, 10.5f,	UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VALID_CONVERSION(float, FLOAT, 10.5f,	UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VALID_CONVERSION(float, FLOAT, 10.5f,	UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VALID_CONVERSION(float, FLOAT, 10.5f,	FLOAT,				10.5f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION(float, FLOAT, 10.5f,	DOUBLE,				10.5,	toDouble)
	
	DECLARE_TEST_VALID_CONVERSION_N(0, float, FLOAT, 1.0f,	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(1, float, FLOAT, 0.0f,	BOOLEAN, false,	toBool)

	// From double to ...
	DECLARE_TEST_VALID_CONVERSION(double, DOUBLE, 10.5,	STRING,				"10.5",	toString)
	DECLARE_TEST_VALID_CONVERSION(double, DOUBLE, 10.5,	INTEGER,			10,		toInt)
	DECLARE_TEST_VALID_CONVERSION(double, DOUBLE, 10.5,	SHORT,				10,		toShort)
	DECLARE_TEST_VALID_CONVERSION(double, DOUBLE, 10.5,	CHAR,				10,		toChar)
	DECLARE_TEST_VALID_CONVERSION(double, DOUBLE, 10.5,	UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VALID_CONVERSION(double, DOUBLE, 10.5,	UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VALID_CONVERSION(double, DOUBLE, 10.5,	UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VALID_CONVERSION(double, DOUBLE, 10.5,	FLOAT,				10.5f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION(double, DOUBLE, 10.5,	DOUBLE,				10.5,	toDouble)

	DECLARE_TEST_VALID_CONVERSION_N(0, double, DOUBLE, 1.0,	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(1, double, DOUBLE, 0.0,	BOOLEAN, false,	toBool)

	// From boolean to ...
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	STRING,				"True",	toString)
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	INTEGER,			1,		toInt)
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	SHORT,				1,		toShort)
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	CHAR,				1,		toChar)
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	UNSIGNED_INTEGER,	1,		toUInt)
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	UNSIGNED_SHORT,		1,		toUShort)
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	UNSIGNED_CHAR,		1,		toUChar)
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	FLOAT,				1.0f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	DOUBLE,				1.0,	toDouble)
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	BOOLEAN,			true,	toBool)

	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	STRING,				"False",	toString)
	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	INTEGER,			0,			toInt)
	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	SHORT,				0,			toShort)
	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	CHAR,				0,			toChar)
	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	UNSIGNED_INTEGER,	0,			toUInt)
	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	UNSIGNED_SHORT,		0,			toUShort)
	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	UNSIGNED_CHAR,		0,			toUChar)
	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	FLOAT,				0.0f,		toFloat)
	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	DOUBLE,				0.0,		toDouble)
	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	BOOLEAN,			false,		toBool)

	// From Vector3 to ...
	DECLARE_TEST_VALID_CONVERSION(Vector3, VECTOR3, Vector3(10.5f, 20.5f, 30.5f), STRING, "(10.5, 20.5, 30.5)", toString)

	// From Quaternion to ...
	DECLARE_TEST_VALID_CONVERSION(Quaternion, QUATERNION, Quaternion(10.5f, 20.5f, 30.5f, 40.5f), STRING, "(10.5, 20.5, 30.5, 40.5)", toString)

	// From Colour to ...
	DECLARE_TEST_VALID_CONVERSION(Color, COLOR, Color(10.5f, 20.5f, 30.5f, 40.5f), STRING, "(10.5, 20.5, 30.5, 40.5)", toString)

	// From Radian to ...
	DECLARE_TEST_VALID_CONVERSION(Radian, RADIAN, Radian(10.5f), STRING, "10.5", toString)

	TEST(ValidConversionFromRadianToDegree)
	{
		Radian val(10.5f);
		Variant v(val);
		CHECK(v.hasType(Variant::RADIAN));
		CHECK(v.convertTo(Variant::DEGREE));
		CHECK(v.hasType(Variant::DEGREE));
		CHECK_CLOSE(val.valueDegrees(), v.toDegree().valueDegrees(), 0.0001f);
	}

	// From Degree to ...
	DECLARE_TEST_VALID_CONVERSION(Degree, DEGREE, Degree(10.5f), STRING, "10.5", toString)

	TEST(ValidConversionFromDegreeToRadian)
	{
		Degree val(10.5f);
		Variant v(val);
		CHECK(v.hasType(Variant::DEGREE));
		CHECK(v.convertTo(Variant::RADIAN));
		CHECK(v.hasType(Variant::RADIAN));
		CHECK_CLOSE(val.valueRadians(), v.toRadian().valueRadians(), 0.0001f);
	}
}

#undef DECLARE_TEST_VALID_CONVERSION
#undef DECLARE_TEST_VALID_CONVERSION_N
#undef DECLARE_TEST_INVALID_CONVERSION



#define DECLARE_TEST_VALID_CONVERSION_N(N, SRC_TYPE, SRC_TYPE_ID, SRC_VALUE, DST_TYPE_ID, DST_VALUE, GETTER)	\
	TEST(ValidConversionFrom##SRC_TYPE_ID##To##DST_TYPE_ID##_##N)							\
	{																						\
		SRC_TYPE val = SRC_VALUE;															\
		Variant v(val);														    			\
																							\
		CHECK(v.hasType(Variant::SRC_TYPE_ID));								    			\
																							\
		CHECK(DST_VALUE ==  v.GETTER());													\
	}


#define DECLARE_TEST_VALID_CONVERSION(SRC_TYPE, SRC_TYPE_ID, SRC_VALUE, DST_TYPE_ID, DST_VALUE, GETTER)	\
	DECLARE_TEST_VALID_CONVERSION_N(0,SRC_TYPE, SRC_TYPE_ID, SRC_VALUE, DST_TYPE_ID, DST_VALUE, GETTER)


#define DECLARE_TEST_INVALID_CONVERSION(SRC_TYPE, SRC_TYPE_ID, SRC_VALUE, DST_TYPE_ID, DST_VALUE, GETTER)	\
	TEST(InvalidConversionFrom##SRC_TYPE_ID##To##DST_TYPE_ID)									\
	{																						\
		SRC_TYPE val = SRC_VALUE;															\
		Variant v(val);																    	\
																							\
		CHECK(v.hasType(Variant::SRC_TYPE_ID));										    	\
																							\
		CHECK(DST_VALUE == v.GETTER());													    \
	}


SUITE(VariantImplicitConversionsSuite)
{
	// Valid conversion: from string to ...
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "test",	STRING,				"test",	toString)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "10",		INTEGER,			10,		toInt)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "10",		SHORT,				10,		toShort)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "10",		CHAR,				10,		toChar)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "10",		UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "10",		UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "10",		UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "10.0",	FLOAT,				10.0f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "10.0",	DOUBLE,				10.0,	toDouble)

	DECLARE_TEST_VALID_CONVERSION_N(0, string, STRING, "True",	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(1, string, STRING, "true",	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(2, string, STRING, "TRUE",	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(3, string, STRING, "False",	BOOLEAN, false,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(4, string, STRING, "false",	BOOLEAN, false,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(5, string, STRING, "FALSE",	BOOLEAN, false,	toBool)

	DECLARE_TEST_VALID_CONVERSION(string, STRING, "(10.0, 20.0, 30.0)",	VECTOR3, Vector3(10.0f, 20.0f, 30.0f), toVector3)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "(10.0, 20.0, 30.0, 40.0)", QUATERNION, Quaternion(10.0f, 20.0f, 30.0f, 40.0f), toQuaternion)
	DECLARE_TEST_VALID_CONVERSION(string, STRING, "(10.0, 20.0, 30.0, 40.0)", COLOR, Color(10.0f, 20.0f, 30.0f, 40.0f), toColor)

	TEST(ValidConversionFromStringToRadian)
	{
		Variant v("10.5f");
		CHECK(v.hasType(Variant::STRING));
		CHECK_CLOSE(10.5f, v.toRadian().valueRadians(), 0.0001f);
	}

	TEST(ValidConversionFromStringToDegree)
	{
		Variant v("10.5");
		CHECK(v.hasType(Variant::STRING));
		CHECK_CLOSE(10.5, v.toDegree().valueDegrees(), 0.0001f);
	}

	// Invalid conversion: from string to ...
	DECLARE_TEST_INVALID_CONVERSION(string, STRING, "test", INTEGER,			0,		toInt)
	DECLARE_TEST_INVALID_CONVERSION(string, STRING, "test", SHORT,				0,		toShort)
	DECLARE_TEST_INVALID_CONVERSION(string, STRING, "test", CHAR,				0,		toChar)
	DECLARE_TEST_INVALID_CONVERSION(string, STRING, "test", UNSIGNED_INTEGER,	0,		toUInt)
	DECLARE_TEST_INVALID_CONVERSION(string, STRING, "test", UNSIGNED_SHORT,		0,		toUShort)	
	DECLARE_TEST_INVALID_CONVERSION(string, STRING, "test", UNSIGNED_CHAR,		0,		toUChar)	
	DECLARE_TEST_INVALID_CONVERSION(string, STRING, "test",	FLOAT,				0.0f,	toFloat)
	DECLARE_TEST_INVALID_CONVERSION(string, STRING, "test",	DOUBLE,				0.0,	toDouble)
	DECLARE_TEST_INVALID_CONVERSION(string, STRING, "test",	BOOLEAN,			false,	toBool)

	// From integer to ...
	DECLARE_TEST_VALID_CONVERSION(int, INTEGER, 10, STRING,				"10",	toString)
	DECLARE_TEST_VALID_CONVERSION(int, INTEGER, 10, INTEGER,			10,		toInt)
	DECLARE_TEST_VALID_CONVERSION(int, INTEGER, 10, SHORT,				10,		toShort)
	DECLARE_TEST_VALID_CONVERSION(int, INTEGER, 10, CHAR,				10,		toChar)
	DECLARE_TEST_VALID_CONVERSION(int, INTEGER, 10,	UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VALID_CONVERSION(int, INTEGER, 10,	UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VALID_CONVERSION(int, INTEGER, 10,	UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VALID_CONVERSION(int, INTEGER, 10,	FLOAT,				10.0f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION(int, INTEGER, 10,	DOUBLE,				10.0,	toDouble)
	
	DECLARE_TEST_VALID_CONVERSION_N(0, int, INTEGER, 1,	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(1, int, INTEGER, 0,	BOOLEAN, false,	toBool)

	// From short to ...
	DECLARE_TEST_VALID_CONVERSION(short, SHORT, 10, STRING,				"10",	toString)
	DECLARE_TEST_VALID_CONVERSION(short, SHORT, 10, INTEGER,			10,		toInt)
	DECLARE_TEST_VALID_CONVERSION(short, SHORT, 10,	SHORT,				10,		toShort)
	DECLARE_TEST_VALID_CONVERSION(short, SHORT, 10, CHAR,				10,		toChar)
	DECLARE_TEST_VALID_CONVERSION(short, SHORT, 10,	UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VALID_CONVERSION(short, SHORT, 10,	UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VALID_CONVERSION(short, SHORT, 10,	UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VALID_CONVERSION(short, SHORT, 10,	FLOAT,				10.0f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION(short, SHORT, 10,	DOUBLE,				10.0,	toDouble)

	DECLARE_TEST_VALID_CONVERSION_N(0, short, SHORT, 1,	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(1, short, SHORT, 0,	BOOLEAN, false,	toBool)

	// From char to ...
	DECLARE_TEST_VALID_CONVERSION(char, CHAR, 10,	STRING,				"10",	toString)
	DECLARE_TEST_VALID_CONVERSION(char, CHAR, 10,	INTEGER,			10,		toInt)
	DECLARE_TEST_VALID_CONVERSION(char, CHAR, 10,	SHORT,				10,		toShort)
	DECLARE_TEST_VALID_CONVERSION(char, CHAR, 10,	CHAR,				10,		toChar)
	DECLARE_TEST_VALID_CONVERSION(char, CHAR, 10,	UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VALID_CONVERSION(char, CHAR, 10,	UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VALID_CONVERSION(char, CHAR, 10,	UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VALID_CONVERSION(char, CHAR, 10,	FLOAT,				10.0f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION(char, CHAR, 10,	DOUBLE,				10.0,	toDouble)

	DECLARE_TEST_VALID_CONVERSION_N(0, char, CHAR, 1,	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(1, char, CHAR, 0,	BOOLEAN, false,	toBool)

	// From unsigned integer to ...
	DECLARE_TEST_VALID_CONVERSION(unsigned int, UNSIGNED_INTEGER, 10,	STRING,				"10",	toString)
	DECLARE_TEST_VALID_CONVERSION(unsigned int, UNSIGNED_INTEGER, 10,	INTEGER,			10,		toInt)
	DECLARE_TEST_VALID_CONVERSION(unsigned int, UNSIGNED_INTEGER, 10,	SHORT,				10,		toShort)
	DECLARE_TEST_VALID_CONVERSION(unsigned int, UNSIGNED_INTEGER, 10,	CHAR,				10,		toChar)
	DECLARE_TEST_VALID_CONVERSION(unsigned int, UNSIGNED_INTEGER, 10,	UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VALID_CONVERSION(unsigned int, UNSIGNED_INTEGER, 10,	UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VALID_CONVERSION(unsigned int, UNSIGNED_INTEGER, 10,	UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VALID_CONVERSION(unsigned int, UNSIGNED_INTEGER, 10,	FLOAT,				10.0f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION(unsigned int, UNSIGNED_INTEGER, 10,	DOUBLE,				10.0,	toDouble)

	DECLARE_TEST_VALID_CONVERSION_N(0, unsigned int, UNSIGNED_INTEGER, 1,	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(1, unsigned int, UNSIGNED_INTEGER, 0,	BOOLEAN, false,	toBool)

	// From unsigned short to ...
	DECLARE_TEST_VALID_CONVERSION(unsigned short, UNSIGNED_SHORT, 10,	STRING,				"10",	toString)
	DECLARE_TEST_VALID_CONVERSION(unsigned short, UNSIGNED_SHORT, 10,	INTEGER,			10,		toInt)
	DECLARE_TEST_VALID_CONVERSION(unsigned short, UNSIGNED_SHORT, 10,	SHORT,				10,		toShort)
	DECLARE_TEST_VALID_CONVERSION(unsigned short, UNSIGNED_SHORT, 10,	CHAR,				10,		toChar)
	DECLARE_TEST_VALID_CONVERSION(unsigned short, UNSIGNED_SHORT, 10,	UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VALID_CONVERSION(unsigned short, UNSIGNED_SHORT, 10,	UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VALID_CONVERSION(unsigned short, UNSIGNED_SHORT, 10,	UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VALID_CONVERSION(unsigned short, UNSIGNED_SHORT, 10,	FLOAT,				10.0f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION(unsigned short, UNSIGNED_SHORT, 10,	DOUBLE,				10.0,	toDouble)

	DECLARE_TEST_VALID_CONVERSION_N(0, unsigned short, UNSIGNED_SHORT, 1,	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(1, unsigned short, UNSIGNED_SHORT, 0,	BOOLEAN, false,	toBool)

	// From unsigned char to ...
	DECLARE_TEST_VALID_CONVERSION(unsigned char, UNSIGNED_CHAR, 10,	STRING,				"10",	toString)
	DECLARE_TEST_VALID_CONVERSION(unsigned char, UNSIGNED_CHAR, 10,	INTEGER,			10,		toInt)
	DECLARE_TEST_VALID_CONVERSION(unsigned char, UNSIGNED_CHAR, 10,	SHORT,				10,		toShort)
	DECLARE_TEST_VALID_CONVERSION(unsigned char, UNSIGNED_CHAR, 10,	CHAR,				10,		toChar)
	DECLARE_TEST_VALID_CONVERSION(unsigned char, UNSIGNED_CHAR, 10,	UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VALID_CONVERSION(unsigned char, UNSIGNED_CHAR, 10,	UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VALID_CONVERSION(unsigned char, UNSIGNED_CHAR, 10,	UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VALID_CONVERSION(unsigned char, UNSIGNED_CHAR, 10,	FLOAT,				10.0f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION(unsigned char, UNSIGNED_CHAR, 10,	DOUBLE,				10.0,	toDouble)

	DECLARE_TEST_VALID_CONVERSION_N(0, unsigned char, UNSIGNED_CHAR, 1,	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(1, unsigned char, UNSIGNED_CHAR, 0,	BOOLEAN, false,	toBool)

	// From float to ...
	DECLARE_TEST_VALID_CONVERSION(float, FLOAT, 10.5f,	STRING,				"10.5",	toString)
	DECLARE_TEST_VALID_CONVERSION(float, FLOAT, 10.5f,	INTEGER,			10,		toInt)
	DECLARE_TEST_VALID_CONVERSION(float, FLOAT, 10.5f,	SHORT,				10,		toShort)
	DECLARE_TEST_VALID_CONVERSION(float, FLOAT, 10.5f,	CHAR,				10,		toChar)
	DECLARE_TEST_VALID_CONVERSION(float, FLOAT, 10.5f,	UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VALID_CONVERSION(float, FLOAT, 10.5f,	UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VALID_CONVERSION(float, FLOAT, 10.5f,	UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VALID_CONVERSION(float, FLOAT, 10.5f,	FLOAT,				10.5f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION(float, FLOAT, 10.5f,	DOUBLE,				10.5,	toDouble)

	DECLARE_TEST_VALID_CONVERSION_N(0, float, FLOAT, 1.0f,	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(1, float, FLOAT, 0.0f,	BOOLEAN, false,	toBool)

	// From double to ...
	DECLARE_TEST_VALID_CONVERSION(double, DOUBLE, 10.5,	STRING,				"10.5",	toString)
	DECLARE_TEST_VALID_CONVERSION(double, DOUBLE, 10.5,	INTEGER,			10,		toInt)
	DECLARE_TEST_VALID_CONVERSION(double, DOUBLE, 10.5,	SHORT,				10,		toShort)
	DECLARE_TEST_VALID_CONVERSION(double, DOUBLE, 10.5,	CHAR,				10,		toChar)
	DECLARE_TEST_VALID_CONVERSION(double, DOUBLE, 10.5,	UNSIGNED_INTEGER,	10,		toUInt)
	DECLARE_TEST_VALID_CONVERSION(double, DOUBLE, 10.5,	UNSIGNED_SHORT,		10,		toUShort)
	DECLARE_TEST_VALID_CONVERSION(double, DOUBLE, 10.5,	UNSIGNED_CHAR,		10,		toUChar)
	DECLARE_TEST_VALID_CONVERSION(double, DOUBLE, 10.5,	FLOAT,				10.5f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION(double, DOUBLE, 10.5,	DOUBLE,				10.5,	toDouble)

	DECLARE_TEST_VALID_CONVERSION_N(0, double, DOUBLE, 1.0,	BOOLEAN, true,	toBool)
	DECLARE_TEST_VALID_CONVERSION_N(1, double, DOUBLE, 0.0,	BOOLEAN, false,	toBool)

	// From boolean to ...
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	STRING,				"True",	toString)
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	INTEGER,			1,		toInt)
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	SHORT,				1,		toShort)
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	CHAR,				1,		toChar)
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	UNSIGNED_INTEGER,	1,		toUInt)
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	UNSIGNED_SHORT,		1,		toUShort)
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	UNSIGNED_CHAR,		1,		toUChar)
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	FLOAT,				1.0f,	toFloat)
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	DOUBLE,				1.0,	toDouble)
	DECLARE_TEST_VALID_CONVERSION_N(0, bool, BOOLEAN, true,	BOOLEAN,			true,	toBool)

	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	STRING,				"False",	toString)
	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	INTEGER,			0,			toInt)
	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	SHORT,				0,			toShort)
	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	CHAR,				0,			toChar)
	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	UNSIGNED_INTEGER,	0,			toUInt)
	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	UNSIGNED_SHORT,		0,			toUShort)
	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	UNSIGNED_CHAR,		0,			toUChar)
	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	FLOAT,				0.0f,		toFloat)
	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	DOUBLE,				0.0,		toDouble)
	DECLARE_TEST_VALID_CONVERSION_N(1, bool, BOOLEAN, false,	BOOLEAN,			false,		toBool)

	// From Vector3 to ...
	DECLARE_TEST_VALID_CONVERSION(Vector3, VECTOR3, Vector3(10.5f, 20.5f, 30.5f), STRING, "(10.5, 20.5, 30.5)", toString)

	// From Quaternion to ...
	DECLARE_TEST_VALID_CONVERSION(Quaternion, QUATERNION, Quaternion(10.5f, 20.5f, 30.5f, 40.5f), STRING, "(10.5, 20.5, 30.5, 40.5)", toString)

	// From Colour to ...
	DECLARE_TEST_VALID_CONVERSION(Color, COLOR, Color(10.5f, 20.5f, 30.5f, 40.5f), STRING, "(10.5, 20.5, 30.5, 40.5)", toString)

	// From Radian to ...
	DECLARE_TEST_VALID_CONVERSION(Radian, RADIAN, Radian(10.5f), STRING, "10.5", toString)

	TEST(ValidConversionFromRadianToDegree)
	{
		Radian val(10.5f);
		Variant v(val);
		CHECK(v.hasType(Variant::RADIAN));
		CHECK_CLOSE(val.valueDegrees(), v.toDegree().valueDegrees(), 0.0001f);
	}

	// From Degree to ...
	DECLARE_TEST_VALID_CONVERSION(Degree, DEGREE, Degree(10.5f), STRING, "10.5", toString)

	TEST(ValidConversionFromDegreeToRadian)
	{
		Degree val(10.5f);
		Variant v(val);
		CHECK(v.hasType(Variant::DEGREE));
		CHECK_CLOSE(val.valueRadians(), v.toRadian().valueRadians(), 0.0001f);
	}
}

#undef DECLARE_TEST_VALID_CONVERSION
#undef DECLARE_TEST_VALID_CONVERSION_N
#undef DECLARE_TEST_INVALID_CONVERSION
