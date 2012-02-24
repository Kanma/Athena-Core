/** @file   SignalsUtils.h
    @author Philip Abbet

    Declaration of the class 'Athena::Signals::SignalsUtils'
*/

#ifndef _ATHENA_SIGNALS_SIGNALSUTILS_H_
#define _ATHENA_SIGNALS_SIGNALSUTILS_H_

#include <Athena-Core/Prerequisites.h>
#include <Athena-Core/Utils/Iterators.h>

namespace Athena {
namespace Signals {


//----------------------------------------------------------------------------------------
/// @brief  Used to convert a signal name (in string form) to a signal ID (usable with
///         the SignalsList class)
///
/// @remark All the methods of this class are static
//----------------------------------------------------------------------------------------
class ATHENA_CORE_SYMBOL SignalsUtils
{
    //_____ Internal types __________
private:
    typedef std::map<std::string, tSignalID>    tIDsList;
    typedef Utils::MapIterator<tIDsList>        tIDsIterator;
    typedef tIDsList::iterator                    tIDsNativeIterator;


    //_____ Methods __________
public:
    static tSignalID getSignalID(const std::string& strName);
    static std::string getSignalName(tSignalID id);


    //_____ Attributes __________
private:
    static tIDsList    m_ids;
};

}
}

#endif
