/** @file   Serialization.h
    @author Philip Abbet

    Serialization / Deserialization functions
*/

#ifndef _ATHENA_DATA_SERIALIZATION_H_
#define _ATHENA_DATA_SERIALIZATION_H_

#include <Athena-Core/Prerequisites.h>
#include <rapidjson/document.h>
#include <rapidjson/rapidjson.h>


namespace Athena {
namespace Data {

    //------------------------------------------------------------------------------------
    /// @brief Returns the rapidjson representation of a Variant (@see
    /// Athena::Utils::Variant)
    ///
    /// @param  pVariant    The variant
    /// @retval value       The resulting rapidjson representation
    /// @param  allocator   The memory allocator to use
    //------------------------------------------------------------------------------------
    ATHENA_CORE_SYMBOL void toJSON(Utils::Variant* pVariant, rapidjson::Value &value,
                                   rapidjson::Value::AllocatorType &allocator);


    //------------------------------------------------------------------------------------
    /// @brief Returns the JSON representation of a describable object (@see
    /// Athena::Utils::Describable)
    //------------------------------------------------------------------------------------
    ATHENA_CORE_SYMBOL std::string toJSON(Utils::Describable* pDescribable);

}
}

#endif
