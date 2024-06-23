/*****************************************************************//**
 * \file   result.h
 * \brief  
 * 
 * \author chaosratel
 * \date   June 2024
 *********************************************************************/

#pragma once

#include <string>
#include <map>
#include <optional>
#include <tuple>
#include <memory>

#include "globals.h"
#include "spdlog/spdlog.h"

//** \brief A code that describes the result of an operation
enum ResultCode
{
    KIFF_SUCCESS = 0,
    KIFF_ERROR = 1,
    KIFF_VALIDATION_LAYER_NOT_PRESENT = 2,
    KIFF_SUITIBLE_PHYSICAL_DEVICE_NOT_FOUND = 3,
    KIFF_CREATE_DEVICE_FAILED = 4,
    KIFF_UNKNOWN = 5,    
    KIFF_UNDEFINED_CODE = 6,
    KIFF_COULD_NOT_FIND_QUEUE_FAMILY_INDICES = 7
};

namespace {
    constexpr const char* RESULT_CODE_STRINGS[] = {    
        "KIFF_SUCCESS",
        "KIFF_ERROR",
        "KIFF_VALIDATION_LAYER_NOT_PRESENT",
        "KIFF_SUITIBLE_PHYSICAL_DEVICE_NOT_FOUND",
        "KIFF_UNKNOWN",
        "KIFF_UNDEFINED_CODE",
        "KIFF_ERROR_COULD_NOT_FIND_QUEUE_FAMILY_INDICES"   
    };
}

/**
 * \brief A Result that contains a result code and the value returned
 * Create a result struct containing the the rusultant value and a code decribing the reult of the operation
 * \tparam T The type of the result value  
 */
template <typename T>
struct Result 
{
    ResultCode code;
    std::optional<T> value;

    /**
     * \brief Constructs a result struct
     * \param c Code describing the operations result
     * \param v A pointer to the results value
     */    
    Result(ResultCode c, std::optional<T> v = std::nullopt ) : code(c), value(std::move(v)) {};
        
    ResultCode getCode() const noexcept 
    {
        return code;
    }

    bool hasValue() const noexcept 
    {
        return value.has_value();
    }

    /**
     * \brief Returns the value if it is valid throws runtime error if no value is found
     * \returns Derefrenced Value T passing ownership to the calling scope
     */    
    T getValue() const 
    {
        if (hasValue())
        {
            return std::move(*value);
        }
        
        spdlog::error(ERROR_RESULT_NO_VALUE_FOUND);
        throw std::runtime_error(ERROR_RESULT_NO_VALUE_FOUND);
    }

    /**
     * \brief Gets the codes description from a map see RESULT_CODE_STRING_MAP
     * \returns The codes description
     */
    const std::string getResultCodeValue() noexcept
    {
        return RESULT_CODE_STRINGS[code];
    }

    /**
     * \brief Gets the codes description from a map see RESULT_CODE_STRING_MAP
     * \returns The codes description
     */
    static const std::string getResultCodeValue(ResultCode code) noexcept
    {
        return RESULT_CODE_STRINGS[code];
    }
};