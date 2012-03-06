/** @file   Variant.cpp
    @author Philip Abbet

    Implementation of the class 'Athena::Utils::Variant'
*/

#include <Athena-Core/Utils/Variant.h>
#include <Athena-Core/Utils/StringConverter.h>
#include <Athena-Math/Vector3.h>
#include <Athena-Math/Quaternion.h>
#include <Athena-Math/Color.h>
#include <sstream>
#include <assert.h>

using namespace Athena;
using namespace Athena::Utils;
using namespace Athena::Math;
using namespace std;


/****************************** CONSTRUCTION / DESTRUCTION *****************************/

//---------------------------------------------------------------------------------------
/// @brief  Constructor
///
/// Construct a null variant of no type
//---------------------------------------------------------------------------------------
Variant::Variant()
: m_type(NONE), m_bNull(true)
{
    memset(&m_value, 0, sizeof(m_value));
}


//---------------------------------------------------------------------------------------
/// @brief  Constructor
///
/// Construct a null variant of the specified type
/// @param  type    The type of the variant
//---------------------------------------------------------------------------------------
Variant::Variant(tType type)
: m_type(type), m_bNull(true)
{
    memset(&m_value, 0, sizeof(m_value));

    if (type == STRUCT)
    {
        m_value._others = new tFieldsMap();
        m_bNull = false;
    }
}


//---------------------------------------------------------------------------------------
/// @brief  Copy constructor
///
/// @param  value   The variant to copy
//---------------------------------------------------------------------------------------
Variant::Variant(const Variant& value)
: m_type(NONE), m_bNull(true)
{
    (*this) = value;
}


void Variant::operator=(const Variant& value)
{
    clear();

    m_type    = value.m_type;
    m_bNull    = value.m_bNull;

    if (m_bNull)
        return;

    switch (value.m_type)
    {
        case STRING:
            m_value._others = new string(*(static_cast<string*>(value.m_value._others)));
            break;

        case VECTOR3:
            m_value._others = new Vector3(*(static_cast<Vector3*>(value.m_value._others)));
            break;

        case QUATERNION:
            m_value._others = new Quaternion(*(static_cast<Quaternion*>(value.m_value._others)));
            break;

        case COLOR:
            m_value._others = new Color(*(static_cast<Color*>(value.m_value._others)));
            break;

        case RADIAN:
            m_value._others = new Radian(*(static_cast<Radian*>(value.m_value._others)));
            break;

        case DEGREE:
            m_value._others = new Degree(*(static_cast<Degree*>(value.m_value._others)));
            break;

        case STRUCT:
            {
                m_value._others = new tFieldsMap();

                tFieldsIterator iter(static_cast<tFieldsMap*>(value.m_value._others)->begin(),
                                     static_cast<tFieldsMap*>(value.m_value._others)->end());
                while (iter.hasMoreElements())
                {
                    Variant* pValue = new Variant(*iter.peekNextValue());
                    setField(iter.peekNextKey(), pValue);
                    iter.moveNext();
                }
                break;
            }

        default:
            m_value = value.m_value;
    }
}


void Variant::clear()
{
    if (!m_bNull)
    {
        switch (m_type)
        {
            case STRING:
                delete static_cast<string*>(m_value._others);
                break;

            case VECTOR3:
                delete static_cast<Vector3*>(m_value._others);
                break;

            case QUATERNION:
                delete static_cast<Quaternion*>(m_value._others);
                break;

            case COLOR:
                delete static_cast<Color*>(m_value._others);
                break;

            case RADIAN:
                delete static_cast<Radian*>(m_value._others);
                break;

            case DEGREE:
                delete static_cast<Degree*>(m_value._others);
                break;

            case STRUCT:
                while (!static_cast<tFieldsMap*>(m_value._others)->empty())
                {
                    delete static_cast<tFieldsMap*>(m_value._others)->begin()->second;
                    static_cast<tFieldsMap*>(m_value._others)->erase(static_cast<tFieldsMap*>(m_value._others)->begin());
                }
                delete static_cast<tFieldsMap*>(m_value._others);
                break;
        }

        memset(&m_value, 0, sizeof(m_value));
    }

    m_bNull = true;
    m_type = NONE;
}


#define DECLARE_CONSTRUCTOR(TYPE, TYPEID, MEMBER)                   \
    Variant::Variant(TYPE value)                                    \
    : m_type(TYPEID), m_bNull(false)                                \
    {                                                               \
        m_value.MEMBER = value;                                     \
    }


#define DECLARE_CONSTRUCTOR_FOR_CLASS(TYPE, TYPEID)                 \
    Variant::Variant(const TYPE& value)                             \
    : m_type(TYPEID), m_bNull(false)                                \
    {                                                               \
        m_value._others = new TYPE(value);                          \
    }


DECLARE_CONSTRUCTOR(int,            INTEGER,          _int)
DECLARE_CONSTRUCTOR(short,          SHORT,            _short)
DECLARE_CONSTRUCTOR(char,           CHAR,             _char)
DECLARE_CONSTRUCTOR(unsigned int,   UNSIGNED_INTEGER, _uint)
DECLARE_CONSTRUCTOR(unsigned short, UNSIGNED_SHORT,   _ushort)
DECLARE_CONSTRUCTOR(unsigned char,  UNSIGNED_CHAR,    _uchar)
DECLARE_CONSTRUCTOR(float,          FLOAT,            _float)
DECLARE_CONSTRUCTOR(double,         DOUBLE,           _double)
DECLARE_CONSTRUCTOR(bool,           BOOLEAN,          _bool)

DECLARE_CONSTRUCTOR_FOR_CLASS(string,       STRING)
DECLARE_CONSTRUCTOR_FOR_CLASS(Vector3,      VECTOR3)
DECLARE_CONSTRUCTOR_FOR_CLASS(Quaternion,   QUATERNION)
DECLARE_CONSTRUCTOR_FOR_CLASS(Color,        COLOR)
DECLARE_CONSTRUCTOR_FOR_CLASS(Radian,       RADIAN)
DECLARE_CONSTRUCTOR_FOR_CLASS(Degree,       DEGREE)


Variant::Variant(const char* strValue)
: m_type(STRING), m_bNull(false)
{
    m_value._others = new string(strValue);
}


#undef DECLARE_CONSTRUCTOR_FOR_MEMBER
#undef DECLARE_CONSTRUCTOR


//---------------------------------------------------------------------------------------
/// @brief  Destructor
//---------------------------------------------------------------------------------------
Variant::~Variant()
{
    clear();
}


/******************************* RETRIEVAL OF THE VALUE ********************************/

#define DECLARE_GETTER(TYPE, TYPEID, METHODNAME, MEMBER, DEFAULTVALUE)          \
    TYPE Variant::METHODNAME() const                                            \
    {                                                                           \
        if (m_type == TYPEID)                                                   \
        {                                                                       \
            if (m_bNull)                                                        \
                return DEFAULTVALUE;                                            \
                                                                                \
            return m_value.MEMBER;                                              \
        }                                                                       \
                                                                                \
        Variant v(*this);                                                       \
        if (v.convertTo(TYPEID))                                                \
            return v.METHODNAME();                                              \
                                                                                \
        return DEFAULTVALUE;                                                    \
    }

#define DECLARE_GETTER_FOR_CLASS(TYPE, TYPEID, METHODNAME, DEFAULTVALUE)        \
    TYPE Variant::METHODNAME() const                                            \
    {                                                                           \
        if (m_type == TYPEID)                                                   \
        {                                                                       \
            if (m_bNull)                                                        \
                return DEFAULTVALUE;                                            \
                                                                                \
            assert(m_value._others);                                            \
            return *(static_cast<TYPE*>(m_value._others));                      \
        }                                                                       \
                                                                                \
        Variant v(*this);                                                       \
        if (v.convertTo(TYPEID))                                                \
            return v.METHODNAME();                                              \
                                                                                \
        return DEFAULTVALUE;                                                    \
    }


DECLARE_GETTER(int,             INTEGER,            toInt,       _int,      0)
DECLARE_GETTER(short,           SHORT,              toShort,     _short,    0)
DECLARE_GETTER(char,            CHAR,               toChar,      _char,     0)
DECLARE_GETTER(unsigned int,    UNSIGNED_INTEGER,   toUInt,      _uint,     0)
DECLARE_GETTER(unsigned short,  UNSIGNED_SHORT,     toUShort,    _ushort,   0)
DECLARE_GETTER(unsigned char,   UNSIGNED_CHAR,      toUChar,     _uchar,    0)
DECLARE_GETTER(float,           FLOAT,              toFloat,     _float,    0.0f)
DECLARE_GETTER(double,          DOUBLE,             toDouble,    _double,   0.0)
DECLARE_GETTER(bool,            BOOLEAN,            toBool,      _bool,     false)

DECLARE_GETTER_FOR_CLASS(string,        STRING,     toString,       "")
DECLARE_GETTER_FOR_CLASS(Vector3,       VECTOR3,    toVector3,      Vector3::ZERO)
DECLARE_GETTER_FOR_CLASS(Quaternion,    QUATERNION, toQuaternion,   Quaternion::ZERO)
DECLARE_GETTER_FOR_CLASS(Color,         COLOR,      toColor,        Color::ZERO)
DECLARE_GETTER_FOR_CLASS(Radian,        RADIAN,     toRadian,       Radian(0.0f))
DECLARE_GETTER_FOR_CLASS(Degree,        DEGREE,     toDegree,       Degree(0.0f))


#undef DECLARE_GETTER_FOR_MEMBER
#undef DECLARE_GETTER


/******************************* STRUCT-RELATED METHODS ********************************/

void Variant::setField(const string& strName, Variant* pValue)
{
    // Assertions
    assert(!strName.empty());
    assert(pValue);
    assert(m_type == STRUCT);
    assert(m_value._others);
    assert(static_cast<tFieldsMap*>(m_value._others));

    // Declarations
    tFieldsMap* pFields = static_cast<tFieldsMap*>(m_value._others);

    // Search the field, and destroy it if found
    tFieldsMap::iterator iter = pFields->find(strName);
    if (iter != pFields->end())
        delete iter->second;

    // Add the value in the list
    (*pFields)[strName] = pValue;
}


Variant* Variant::getField(const string& strName)
{
    // Assertions
    assert(!strName.empty());
    assert(m_type == STRUCT);
    assert(m_value._others);
    assert(static_cast<tFieldsMap*>(m_value._others));

    // Declarations
    tFieldsMap* pFields = static_cast<tFieldsMap*>(m_value._others);

    // Search the field
    tFieldsMap::iterator iter = pFields->find(strName);
    if (iter != pFields->end())
        return iter->second;

    // Not found
    return 0;
}


Variant::tFieldsIterator Variant::getFieldsIterator()
{
    // Assertions
    assert(m_type == STRUCT);
    assert(m_value._others);
    assert(static_cast<tFieldsMap*>(m_value._others));

    // Declarations
    tFieldsMap* pFields = static_cast<tFieldsMap*>(m_value._others);

    return tFieldsIterator(pFields->begin(), pFields->end());
}


/********************************** TYPE CONVERSION ************************************/

#define DECLARE_CONVERSION_FROM_STRING(DST_TYPEID, DST_MEMBER)                  \
    case DST_TYPEID:                                                            \
    {                                                                           \
        string* p = static_cast<string*>(m_value._others);                      \
        std::istringstream str(*p);                                             \
        str >> m_value.DST_MEMBER;                                              \
                                                                                \
        if (str.fail())                                                         \
        {                                                                       \
            m_value._others = p;                                                \
            return false;                                                       \
        }                                                                       \
                                                                                \
        delete p;                                                               \
        m_type = type;                                                          \
        return true;                                                            \
    }


#define DECLARE_CONVERSION_TO_STRING(SRC_MEMBER)                                \
    case STRING:                                                                \
        {                                                                       \
            m_type = type;                                                      \
            string* p = new string();                                           \
            *p = StringConverter::toString(m_value.SRC_MEMBER);                 \
            m_value._others = p;                                                \
            return true;                                                        \
        }


#define DECLARE_CONVERSION_TO_STRING_WITH_CAST(SRC_MEMBER, TYPE)                \
    case STRING:                                                                \
        {                                                                       \
            m_type = type;                                                      \
            string* p = new string();                                           \
            *p = StringConverter::toString((TYPE) m_value.SRC_MEMBER);          \
            m_value._others = p;                                                \
            return true;                                                        \
        }


#define DECLARE_CONVERSION_BY_CAST(SRC_MEMBER, DST_TYPE, DST_TYPEID, DST_MEMBER)    \
    case DST_TYPEID:                                                                \
        m_type = type;                                                              \
        m_value.DST_MEMBER = (DST_TYPE) m_value.SRC_MEMBER;                         \
        return true;


#define DECLARE_CONVERSION_TO_BOOLEAN(SRC_MEMBER)                                   \
    case BOOLEAN:                                                                   \
        m_type = type;                                                              \
        m_value._bool = (m_value.SRC_MEMBER != 0);                                  \
        return true;


#define DECLARE_CONVERSION_FROM_BOOLEAN(DST_TYPE, DST_TYPEID, DST_MEMBER)           \
    case DST_TYPEID:                                                                \
        m_type = type;                                                              \
        m_value.DST_MEMBER = (DST_TYPE) (m_value._bool ? 1 : 0);                    \
        return true;



bool Variant::convertTo(tType type)
{
    // Test if the desired type is the same as the current one
    if (m_type == type)
        return true;

    // If null, rests null
    if (isNull())
    {
        m_type = type;
        return true;
    }

    // Performs the conversion
    switch (m_type)
    {
        // String to TYPE
    case STRING:
        assert(m_value._others);

        switch (type)
        {
            DECLARE_CONVERSION_FROM_STRING(INTEGER,             _int)
            DECLARE_CONVERSION_FROM_STRING(SHORT,               _short)
            DECLARE_CONVERSION_FROM_STRING(UNSIGNED_INTEGER,    _uint)
            DECLARE_CONVERSION_FROM_STRING(UNSIGNED_SHORT,      _ushort)
            DECLARE_CONVERSION_FROM_STRING(FLOAT,               _float)
            DECLARE_CONVERSION_FROM_STRING(DOUBLE,              _double)


        case CHAR:
            {
                string* p = static_cast<string*>(m_value._others);
                std::istringstream str(*p);

                int temp;
                str >> temp;

                if (str.fail())
                    return false;

                delete p;

                m_type = type;
                m_value._char = (char) temp;
                return true;
            }

        case UNSIGNED_CHAR:
            {
                string* p = static_cast<string*>(m_value._others);
                std::istringstream str(*p);

                unsigned int temp;
                str >> temp;

                if (str.fail())
                    return false;

                delete p;

                m_type = type;
                m_value._uchar = (unsigned char) temp;
                return true;
            }

        case BOOLEAN:
            {
                string* p = static_cast<string*>(m_value._others);

                if ((*p == "True") || (*p == "true") || (*p == "TRUE"))
                {
                    delete p;
                    m_type = type;
                    m_value._bool = true;
                    return true;
                }
                else if ((*p == "False") || (*p == "false") || (*p == "FALSE"))
                {
                    delete p;
                    m_type = type;
                    m_value._bool = false;
                    return true;
                }

                return false;
            }

        case VECTOR3:
            {
                string* p = static_cast<string*>(m_value._others);
                char c;
                float x, y, z;
                std::istringstream str(*p);

                str >> c >> x >> c >> y >> c >> z >> c;
                if (str.fail())
                    return false;

                delete p;

                m_type = type;
                m_value._others = new Vector3(x, y, z);
                return true;
            }

        case QUATERNION:
            {
                string* p = static_cast<string*>(m_value._others);
                char c;
                float w, x, y, z;
                std::istringstream str(*p);

                str >> c >> w >> c >> x >> c >> y >> c >> z >> c;
                if (str.fail())
                    return false;

                delete p;

                m_type = type;
                m_value._others = new Quaternion(w, x, y, z);
                return true;
            }

        case COLOR:
            {
                string* p = static_cast<string*>(m_value._others);
                char c;
                float r, g, b, a;
                std::istringstream str(*p);

                str >> c >> r >> c >> g >> c >> b >> c >> a >> c;
                if (str.fail())
                    return false;

                delete p;

                m_type = type;
                m_value._others = new Color(r, g, b, a);
                return true;
            }

        case RADIAN:
            {
                string* p = static_cast<string*>(m_value._others);
                float a;
                std::istringstream str(*p);

                str >> a;
                if (str.fail())
                    return false;

                delete p;

                m_type = type;
                m_value._others = new Radian(a);
                return true;
            }

        case DEGREE:
            {
                string* p = static_cast<string*>(m_value._others);
                float a;
                std::istringstream str(*p);

                str >> a;
                if (str.fail())
                    return false;

                delete p;

                m_type = type;
                m_value._others = new Degree(a);
                return true;
            }
        }
        break;


        // Integer to TYPE
    case INTEGER:
        switch (type)
        {
            DECLARE_CONVERSION_TO_STRING(_int)

            DECLARE_CONVERSION_BY_CAST(_int, short,             SHORT,              _short)
            DECLARE_CONVERSION_BY_CAST(_int, char,              CHAR,               _char)
            DECLARE_CONVERSION_BY_CAST(_int, unsigned int,      UNSIGNED_INTEGER,   _uint)
            DECLARE_CONVERSION_BY_CAST(_int, unsigned short,    UNSIGNED_SHORT,     _ushort)
            DECLARE_CONVERSION_BY_CAST(_int, unsigned char,     UNSIGNED_CHAR,      _uchar)
            DECLARE_CONVERSION_BY_CAST(_int, float,             FLOAT,              _float)
            DECLARE_CONVERSION_BY_CAST(_int, double,            DOUBLE,             _double)
            DECLARE_CONVERSION_TO_BOOLEAN(_int)
        }
        break;


        // Short to TYPE
    case SHORT:
        switch (type)
        {
            DECLARE_CONVERSION_TO_STRING(_short)

            DECLARE_CONVERSION_BY_CAST(_short, int,             INTEGER,            _int)
            DECLARE_CONVERSION_BY_CAST(_short, char,            CHAR,               _char)
            DECLARE_CONVERSION_BY_CAST(_short, unsigned int,    UNSIGNED_INTEGER,   _uint)
            DECLARE_CONVERSION_BY_CAST(_short, unsigned short,  UNSIGNED_SHORT,     _ushort)
            DECLARE_CONVERSION_BY_CAST(_short, unsigned char,   UNSIGNED_CHAR,      _uchar)
            DECLARE_CONVERSION_BY_CAST(_short, float,           FLOAT,              _float)
            DECLARE_CONVERSION_BY_CAST(_short, double,          DOUBLE,             _double)
            DECLARE_CONVERSION_TO_BOOLEAN(_short)
        }
        break;


        // Char to TYPE
    case CHAR:
        switch (type)
        {
            DECLARE_CONVERSION_TO_STRING_WITH_CAST(_char, int)

            DECLARE_CONVERSION_BY_CAST(_char, int,              INTEGER,            _int)
            DECLARE_CONVERSION_BY_CAST(_char, short,            SHORT,              _short)
            DECLARE_CONVERSION_BY_CAST(_char, unsigned int,     UNSIGNED_INTEGER,   _uint)
            DECLARE_CONVERSION_BY_CAST(_char, unsigned short,   UNSIGNED_SHORT,     _ushort)
            DECLARE_CONVERSION_BY_CAST(_char, unsigned char,    UNSIGNED_CHAR,      _uchar)
            DECLARE_CONVERSION_BY_CAST(_char, float,            FLOAT,              _float)
            DECLARE_CONVERSION_BY_CAST(_char, double,           DOUBLE,             _double)
            DECLARE_CONVERSION_TO_BOOLEAN(_char)
        }
        break;


        // Unsigned integer to TYPE
    case UNSIGNED_INTEGER:
        switch (type)
        {
            DECLARE_CONVERSION_TO_STRING(_uint)

            DECLARE_CONVERSION_BY_CAST(_uint, int,              INTEGER,            _int)
            DECLARE_CONVERSION_BY_CAST(_uint, short,            SHORT,              _short)
            DECLARE_CONVERSION_BY_CAST(_uint, char,             CHAR,               _char)
            DECLARE_CONVERSION_BY_CAST(_uint, unsigned short,   UNSIGNED_SHORT,     _ushort)
            DECLARE_CONVERSION_BY_CAST(_uint, unsigned char,    UNSIGNED_CHAR,      _uchar)
            DECLARE_CONVERSION_BY_CAST(_uint, float,            FLOAT,              _float)
            DECLARE_CONVERSION_BY_CAST(_uint, double,           DOUBLE,             _double)
            DECLARE_CONVERSION_TO_BOOLEAN(_uint)
        }
        break;


        // Unsigned short to TYPE
    case UNSIGNED_SHORT:
        switch (type)
        {
            DECLARE_CONVERSION_TO_STRING(_ushort)

            DECLARE_CONVERSION_BY_CAST(_ushort, int,            INTEGER,            _int)
            DECLARE_CONVERSION_BY_CAST(_ushort, short,          SHORT,              _short)
            DECLARE_CONVERSION_BY_CAST(_ushort, char,           CHAR,               _char)
            DECLARE_CONVERSION_BY_CAST(_ushort, unsigned int,   UNSIGNED_INTEGER,   _uint)
            DECLARE_CONVERSION_BY_CAST(_ushort, unsigned char,  UNSIGNED_CHAR,      _uchar)
            DECLARE_CONVERSION_BY_CAST(_ushort, float,          FLOAT,              _float)
            DECLARE_CONVERSION_BY_CAST(_ushort, double,         DOUBLE,             _double)
            DECLARE_CONVERSION_TO_BOOLEAN(_ushort)
        }
        break;


        // Unsigned char to TYPE
    case UNSIGNED_CHAR:
        switch (type)
        {
            DECLARE_CONVERSION_TO_STRING_WITH_CAST(_uchar, unsigned int)

            DECLARE_CONVERSION_BY_CAST(_uchar, int,             INTEGER,            _int)
            DECLARE_CONVERSION_BY_CAST(_uchar, short,           SHORT,              _short)
            DECLARE_CONVERSION_BY_CAST(_uchar, char,            CHAR,               _char)
            DECLARE_CONVERSION_BY_CAST(_uchar, unsigned int,    UNSIGNED_INTEGER,   _uint)
            DECLARE_CONVERSION_BY_CAST(_uchar, unsigned short,  UNSIGNED_SHORT,     _ushort)
            DECLARE_CONVERSION_BY_CAST(_uchar, float,           FLOAT,              _float)
            DECLARE_CONVERSION_BY_CAST(_uchar, double,          DOUBLE,             _double)
            DECLARE_CONVERSION_TO_BOOLEAN(_uchar)
        }
        break;


        // Float to TYPE
    case FLOAT:
        switch (type)
        {
        case BOOLEAN:
            m_type = type;
            m_value._bool = !MathUtils::RealEqual(m_value._float, 0.0f);
            return true;

            DECLARE_CONVERSION_TO_STRING_WITH_CAST(_float, Real)

            DECLARE_CONVERSION_BY_CAST(_float, int,             INTEGER,            _int)
            DECLARE_CONVERSION_BY_CAST(_float, short,           SHORT,              _short)
            DECLARE_CONVERSION_BY_CAST(_float, char,            CHAR,               _char)
            DECLARE_CONVERSION_BY_CAST(_float, unsigned int,    UNSIGNED_INTEGER,   _uint)
            DECLARE_CONVERSION_BY_CAST(_float, unsigned short,  UNSIGNED_SHORT,     _ushort)
            DECLARE_CONVERSION_BY_CAST(_float, unsigned char,   UNSIGNED_CHAR,      _uchar)
            DECLARE_CONVERSION_BY_CAST(_float, double,          DOUBLE,             _double)
        }
        break;


        // Double to TYPE
    case DOUBLE:
        switch (type)
        {
        case BOOLEAN:
            m_type = type;
            m_value._bool = !MathUtils::RealEqual((Real) m_value._double, (Real) 0.0);
            return true;

            DECLARE_CONVERSION_TO_STRING_WITH_CAST(_double, Real)

            DECLARE_CONVERSION_BY_CAST(_double, int,            INTEGER,            _int)
            DECLARE_CONVERSION_BY_CAST(_double, short,          SHORT,              _short)
            DECLARE_CONVERSION_BY_CAST(_double, char,           CHAR,               _char)
            DECLARE_CONVERSION_BY_CAST(_double, unsigned int,   UNSIGNED_INTEGER,   _uint)
            DECLARE_CONVERSION_BY_CAST(_double, unsigned short, UNSIGNED_SHORT,     _ushort)
            DECLARE_CONVERSION_BY_CAST(_double, unsigned char,  UNSIGNED_CHAR,      _uchar)
            DECLARE_CONVERSION_BY_CAST(_double, float,          FLOAT,              _float)
        }
        break;


        // Boolean to TYPE
    case BOOLEAN:
        switch (type)
        {
        case STRING:
            {
                m_type = type;
                string* p = new string();
                *p = (m_value._bool ? "True" : "False");
                m_value._others = p;
                return true;
            }

            DECLARE_CONVERSION_FROM_BOOLEAN(int,            INTEGER,                _uint)
            DECLARE_CONVERSION_FROM_BOOLEAN(short,          SHORT,                  _short)
            DECLARE_CONVERSION_FROM_BOOLEAN(char,           CHAR,                   _char)
            DECLARE_CONVERSION_FROM_BOOLEAN(unsigned int,   UNSIGNED_INTEGER,       _uint)
            DECLARE_CONVERSION_FROM_BOOLEAN(unsigned short, UNSIGNED_SHORT,         _ushort)
            DECLARE_CONVERSION_FROM_BOOLEAN(unsigned char,  UNSIGNED_CHAR,          _uchar)
            DECLARE_CONVERSION_FROM_BOOLEAN(float,          FLOAT,                  _float)
            DECLARE_CONVERSION_FROM_BOOLEAN(double,         DOUBLE,                 _double)
        }
        break;


        // Vector3 to TYPE
    case VECTOR3:
        switch (type)
        {
        case STRING:
            {
                m_type = type;
                Vector3* v = static_cast<Vector3*>(m_value._others);
                string* s = new string();
                *s += "(" + StringConverter::toString(v->x) + ", " +
                      StringConverter::toString(v->y) + ", " + StringConverter::toString(v->z) + ")";
                delete v;
                m_value._others = s;
                return true;
            }
        }
        break;


        // Quaternion to TYPE
    case QUATERNION:
        switch (type)
        {
        case STRING:
            {
                m_type = type;
                Quaternion* q = static_cast<Quaternion*>(m_value._others);
                string* s = new string();
                *s += "(" + StringConverter::toString(q->w) + ", " + StringConverter::toString(q->x) + ", " +
                      StringConverter::toString(q->y) + ", " + StringConverter::toString(q->z) + ")";
                delete q;
                m_value._others = s;
                return true;
            }
        }
        break;


        // Color to TYPE
    case COLOR:
        switch (type)
        {
        case STRING:
            {
                m_type = type;
                Color* c = static_cast<Color*>(m_value._others);
                string* s = new string();
                *s += "(" + StringConverter::toString(c->r) + ", " + StringConverter::toString(c->g) + ", " +
                      StringConverter::toString(c->b) + ", " + StringConverter::toString(c->a) + ")";
                delete c;
                m_value._others = s;
                return true;
            }
        }
        break;


        // Radian to TYPE
    case RADIAN:
        switch (type)
        {
        case STRING:
            {
                m_type = type;
                Radian* a = static_cast<Radian*>(m_value._others);
                string* s = new string();
                *s = StringConverter::toString(a->valueRadians());
                delete a;
                m_value._others = s;
                return true;
            }

        case DEGREE:
            {
                m_type = type;
                Radian* a = static_cast<Radian*>(m_value._others);
                m_value._others = new Degree(*a);
                delete a;
                return true;
            }
        }
        break;


        // Degree to TYPE
    case DEGREE:
        switch (type)
        {
        case STRING:
            {
                m_type = type;
                Degree* a = static_cast<Degree*>(m_value._others);
                string* s = new string();
                *s = StringConverter::toString(a->valueDegrees());
                delete a;
                m_value._others = s;
                return true;
            }

        case RADIAN:
            {
                m_type = type;
                Degree* a = static_cast<Degree*>(m_value._others);
                m_value._others = new Radian(*a);
                delete a;
                return true;
            }
        }
        break;
    }

    // Unknown conversion
    return false;
}
