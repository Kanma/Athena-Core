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
    /// @brief Returns the Variant represented by a rapidjson value (@see
    /// Athena::Utils::Variant)
    ///
    /// @param  value       The rapidjson value
    /// @retval pVariant    The variant
    //------------------------------------------------------------------------------------
    ATHENA_CORE_SYMBOL void fromJSON(const rapidjson::Value& value, Utils::Variant* pVariant);


    //------------------------------------------------------------------------------------
    /// @brief Returns the rapidjson representation of a describable object (@see
    /// Athena::Utils::Describable)
    ///
    /// @param  pDescribable        The describable
    /// @retval json_describable    The resulting rapidjson representation
    /// @param  allocator           The memory allocator to use
    //------------------------------------------------------------------------------------
    ATHENA_CORE_SYMBOL void toJSON(Utils::Describable* pDescribable,
                                   rapidjson::Value &json_describable,
                                   rapidjson::Value::AllocatorType &allocator);


    //------------------------------------------------------------------------------------
    /// @brief Returns the Describable object represented by a rapidjson value (@see
    /// Athena::Utils::Describable)
    ///
    /// @param json_describable     The rapidjson value
    /// @retval pDescribable        The describable
    //------------------------------------------------------------------------------------
    ATHENA_CORE_SYMBOL void fromJSON(const rapidjson::Value& json_describable,
                                     Utils::Describable* pDescribable,
                                     Utils::PropertiesList* pDelayedProperties = 0);


    //------------------------------------------------------------------------------------
    /// @brief Returns the JSON representation of a describable object (@see
    /// Athena::Utils::Describable) as a string
    //------------------------------------------------------------------------------------
    ATHENA_CORE_SYMBOL std::string toJSON(Utils::Describable* pDescribable);

}
}

#endif
