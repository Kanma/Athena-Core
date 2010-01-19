/** @file		Declarations.h
	@author		Philip Abbet

	Definition of constants and type related to the Log Manager
*/

#ifndef _ATHENA_LOG_DECLARATIONS_H_
#define _ATHENA_LOG_DECLARATIONS_H_

#include <Athena-Core/Prerequisites.h>


namespace Athena {
namespace Log {

	// To use the following macros, you must have a string constant named __CONTEXT__ declared
#if ATHENA_CORE_DETAILED_LOGS
	/// Log a comment
	#define ATHENA_LOG_COMMENT(message)	LogManager::getSingletonPtr()->log(LOG_COMMENT, __CONTEXT__, (message), __FILE__, __FUNCTION__, __LINE__)
	
	/// Log a debug message
	#define ATHENA_LOG_DEBUG(message)	LogManager::getSingletonPtr()->log(LOG_DEBUG, __CONTEXT__, (message), __FILE__, __FUNCTION__, __LINE__)
#else
	/// Log a comment
	#define ATHENA_LOG_COMMENT(message)

	/// Log a debug message
	#define ATHENA_LOG_DEBUG(message)
#endif
	/// Log a warning
	#define ATHENA_LOG_WARNING(message)	LogManager::getSingletonPtr()->log(LOG_WARNING, __CONTEXT__, (message), __FILE__, __FUNCTION__, __LINE__)

	/// Log an error
	#define ATHENA_LOG_ERROR(message)	LogManager::getSingletonPtr()->log(LOG_ERROR, __CONTEXT__, (message), __FILE__, __FUNCTION__, __LINE__)

	/// Log an event
	#define ATHENA_LOG_EVENT(message)	LogManager::getSingletonPtr()->log(LOG_EVENT, __CONTEXT__, (message), __FILE__, __FUNCTION__, __LINE__)


#if ATHENA_CORE_DETAILED_LOGS
	/// Log a comment in the specified context
	#define ATHENA_LOG_COMMENT2(context, message)	LogManager::getSingletonPtr()->log(LOG_COMMENT, context, (message), __FILE__, __FUNCTION__, __LINE__)

	/// Log a debug message in the specified context
	#define ATHENA_LOG_DEBUG2(context, message)		LogManager::getSingletonPtr()->log(LOG_DEBUG, context, (message), __FILE__, __FUNCTION__, __LINE__)
#else
	/// Log a comment in the specified context
	#define ATHENA_LOG_COMMENT2(context, message)

	/// Log a debug message in the specified context
	#define ATHENA_LOG_DEBUG2(context, message)
#endif
	/// Log a warning in the specified context
	#define ATHENA_LOG_WARNING2(context, message)	LogManager::getSingletonPtr()->log(LOG_WARNING, context, (message), __FILE__, __FUNCTION__, __LINE__)

	/// Log an error in the specified context
	#define ATHENA_LOG_ERROR2(context, message)		LogManager::getSingletonPtr()->log(LOG_ERROR, context, (message), __FILE__, __FUNCTION__, __LINE__)

	/// Log an event in the specified context
	#define ATHENA_LOG_EVENT2(context, message)		LogManager::getSingletonPtr()->log(LOG_EVENT, context, (message), __FILE__, __FUNCTION__, __LINE__)


//---------------------------------------------------------------------------------------
/// @brief	The levels of the log messages
//---------------------------------------------------------------------------------------
enum tMessageType
{
	LOG_COMMENT,	///< A comment
	LOG_DEBUG,		///< A debug message
	LOG_WARNING,	///< A warning
	LOG_ERROR,		///< An error
	LOG_EVENT		///< An event
};

}
}

#endif
