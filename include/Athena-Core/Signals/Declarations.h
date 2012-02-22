/**    @file    Declarations.h
    @author    Philip Abbet

    Declaration of the base signal IDs
*/

#ifndef _ATHENA_SIGNALS_DECLARATIONS_H_
#define _ATHENA_SIGNALS_DECLARATIONS_H_

namespace Athena {
namespace Signals {


//----------------------------------------------------------------------------------------
/// @brief    Enumeration of the reserved signal IDs ranges
//----------------------------------------------------------------------------------------
enum tSignals
{
    SIGNALS_ENTITIES    = 0,        ///< Start of the range of IDs reserved for Athena-Entities
    SIGNALS_PHYSICS     = 100,      ///< Start of the range of IDs reserved for Athena-Physics
    SIGNALS_GRAPHICS    = 200,      ///< Start of the range of IDs reserved for Athena-Graphics
    SIGNALS_AUDIO       = 300,      ///< Start of the range of IDs reserved for Athena-Audio
    SIGNALS_GUI         = 400,      ///< Start of the range of IDs reserved for Athena-GUI
    SIGNAL_STRINGS      = 10000,    ///< Start of the range of IDs reserved for the SignalsUtils class
    SIGNAL_APPLICATION  = 20000,    ///< Start of the range of IDs reserved for the application
};

}
}

#endif
