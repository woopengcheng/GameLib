/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tencent is pleased to support the open source community by making behaviac available.
//
// Copyright (C) 2015 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef BEHAVIAC_BASE_CORE_TYPES_H
#define BEHAVIAC_BASE_CORE_TYPES_H

#include "behaviac/base/core/config.h"
#include "behaviac/base/core/staticassert.h"

#if defined(_MSC_VER) && _MSC_VER < 1600 /* MSVC2010 */
#include "behaviac/base/core/msc_stdint.h"
#else
#include <stdint.h>
#endif

#include <stddef.h>
#include <float.h>
#include <math.h>

/*! \addtogroup core
 * @{
 * \addtogroup types
 * various utilies, such as BEHAVIAC_ALIGNED, BEHAVIAC_ROUND and many many more
 * @{ */

namespace behaviac
{
    typedef char				Char;
    typedef float				Float32;
    typedef double				Float64;
#if BEHAVIAC_COMPILER_64BITS
    typedef uint64_t            Address;    //< Adress unsigned word (the size of the ADRESS bus)
#else
    typedef unsigned long       Address;    //< Address unsigned word (the size of the ADRESS bus)
#endif
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // behaviac min/max value for each types
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    const int8_t	S8_Min = -128;                         //< Minimum value for a signed 8 bits integer
    const int8_t	S8_Max = 127;                          //< Maximum value for a signed 8 bits integer
    const uint8_t	U8_Min = 0;                            //< Minimum value for a unsigned 8 bits integer
    const uint8_t	U8_Max = 255;                          //< Maximum value for a unsigned 8 bits integer
    const int16_t	S16_Min = -32768;                       //< Minimum value for a signed 16 bits integer
    const int16_t	S16_Max = 32767;                        //< Maximum value for a signed 16 bits integer
    const uint16_t	U16_Min = 0;                            //< Minimum value for a unsigned 16 bits integer
    const uint16_t	U16_Max = 65535;                        //< Maximum value for a unsigned 16 bits integer
    const int32_t	S32_Min = 0x80000000;                   //< Minimum value for a signed 32 bits integer
    const int32_t	S32_Max = 0x7FFFFFFF;                   //< Maximum value for a signed 32 bits integer
    const uint32_t	U32_Min = 0x00000000;                   //< Minimum value for a unsigned 32 bits integer
    const uint32_t	U32_Max = 0xFFFFFFFF;                   //< Maximum value for a unsigned 32 bits integer
    const int64_t	S64_Min = 0x8000000000000000LL;         //< Minimum value for a signed 64 bits integer
    const int64_t	S64_Max = 0x7FFFFFFFFFFFFFFFLL;         //< Maximum value for a signed 64 bits integer
    const uint64_t	U64_Min = 0x0000000000000000ULL;        //< Minimum value for a unsigned 64 bits integer
    const uint64_t	U64_Max = 0xFFFFFFFFFFFFFFFFULL;        //< Maximum value for a unsigned 64 bits integer

    const Float32	Float32_Min = -3.402823466E+38F;            //< Minimum value for a 32 bits float
    const Float32	Float32_Max = 3.402823466E+38F;             //< Maximum value for a 32 bits float
    const Float32	Float32_Positive_Min = 1.175494351E-38F;             //< Smallest representable positive value for a 32 bits float
    const Float32	Float32_Positive_Max = 3.402823466E+38F;             //< Biggest representable positive value for a 32 bits float
    const int32_t   Float32_MinExp = -126;                         //< Minimum binary exponent for 32 bit float
    const int32_t   Float32_MaxExp = 127;                          //< Maximum binary exponent for 32 bit float

    const Float32	Float32_Epsilon = 0.000001f;

    inline void GetTypeMin(int8_t& Val)
    {
        Val = S8_Min;     //< Set Val to the minimal value for a signed 8 bits integer
    }
    inline void GetTypeMax(int8_t& Val)
    {
        Val = S8_Max;     //< Set Val to the maximum value for a signed 8 bits integer
    }
    inline void GetTypeMin(uint8_t& Val)
    {
        Val = U8_Min;     //< Set Val to the minimal value for a unsigned 8 bits integer
    }
    inline void GetTypeMax(uint8_t& Val)
    {
        Val = U8_Max;     //< Set Val to the maximum value for a unsigned 8 bits integer
    }
    inline void GetTypeMin(int16_t& Val)
    {
        Val = S16_Min;    //< Set Val to the minimal value for a signed 16 bits integer
    }
    inline void GetTypeMax(int16_t& Val)
    {
        Val = S16_Max;    //< Set Val to the maximum value for a signed 16 bits integer
    }
    inline void GetTypeMin(uint16_t& Val)
    {
        Val = U16_Min;    //< Set Val to the minimal value for a unsigned 16 bits integer
    }
    inline void GetTypeMax(uint16_t& Val)
    {
        Val = U16_Max;    //< Set Val to the maximum value for a unsigned 16 bits integer
    }
    inline void GetTypeMin(int32_t& Val)
    {
        Val = S32_Min;    //< Set Val to the minimal value for a signed 32 bits integer
    }
    inline void GetTypeMax(int32_t& Val)
    {
        Val = S32_Max;    //< Set Val to the maximum value for a signed 32 bits integer
    }
    inline void GetTypeMin(uint32_t& Val)
    {
        Val = U32_Min;    //< Set Val to the minimal value for a unsigned 32 bits integer
    }
    inline void GetTypeMax(uint32_t& Val)
    {
        Val = U32_Max;    //< Set Val to the maximum value for a unsigned 32 bits integer
    }
    inline void GetTypeMin(int64_t& Val)
    {
        Val = S64_Min;    //< Set Val to the minimal value for a signed 64 bits integer
    }
    inline void GetTypeMax(int64_t& Val)
    {
        Val = S64_Max;    //< Set Val to the maximum value for a signed 64 bits integer
    }
    inline void GetTypeMin(uint64_t& Val)
    {
        Val = U64_Min;    //< Set Val to the minimal value for a unsigned 64 bits integer
    }
    inline void GetTypeMax(uint64_t& Val)
    {
        Val = U64_Max;    //< Set Val to the maximum value for a unsigned 64 bits integer
    }

    inline void GetTypeMin(Float32& Val)
    {
        Val = Float32_Min;    //< Set Val to the minimal value for a 32 bits float
    }
    inline void GetTypeMax(Float32& Val)
    {
        Val = Float32_Max;    //< Set Val to the maximum value for a 32 bits float
    }

    inline int _finite(Float32 x)
    {
#if BEHAVIAC_COMPILER_MSVC
        return ::_finite(x);
#elif BEHAVIAC_COMPILER_APPLE

        if (x <= -INFINITY || x >= INFINITY)
        {
            return 0;
        }

        return x;
#else
        return finite(x);
#endif//#if BEHAVIAC_COMPILER_MSVC
    }

    inline int _finite(Float64 x)
    {
#if BEHAVIAC_COMPILER_MSVC
        return ::_finite(x);
#elif BEHAVIAC_COMPILER_APPLE

        if (x <= -INFINITY || x >= INFINITY)
        {
            return 0;
        }

        return x;
#else
        return finite(x);
#endif//#if BEHAVIAC_COMPILER_MSVC
    }

#define	IsQNaN(Value)				((Value)!=(Value))					// Because NaN is equal to itself

    /////////////////////////////////////////////////////////////////
    ///// This section, is functions for F32                    /////
    /////////////////////////////////////////////////////////////////

    /// Calculates the absolute value of the floating-point argument
    inline Float32 GetAbsolute(Float32 x)
    {
        return fabsf(x);
    }

    inline Float64 GetAbsolute(Float64 x)
    {
        return fabs(x);
    }

    /// Convert a F32 value to an int32_t
    inline int32_t GetInteger(Float32 x)
    {
        // Important note: this can trigger a floating point exception on PSP/PS3 if
        // the value of the F32 is > then S32_Max (0x7FFFFFFF)
        return (int32_t)x;
    }

    inline int64_t GetInteger(Float64 x)
    {
        // Important note: this can trigger a floating point exception on PSP/PS3 if
        // the value of the F32 is > then S32_Max (0x7FFFFFFF)
        return (int64_t)x;
    }

#define BEHAVIAC_ARRAY_NELEMENT(Array) (sizeof(Array)/sizeof(Array[0]))

    /// Add a index to a pointer and return it casted to type
    /*! \param type     The return type (typically another pointer)
    \param Ptr      The pointer on which to add the offset
    \param Ofs      Offset to add in BYTES (not in Pointer element type value)
    \return A pointer of type 'type', pointer at the address of Ptr + Ofs
    */
#define BEHAVIAC_ADD_PTR(type, Ptr, Ofs)            ((type) (((size_t)(Ptr)) + (size_t)(Ofs)))

    /// Subtract a index from a pointer and return it casted to type
    /*! \param type     The return type (typically another pointer)
    \param Ptr      The pointer on which to subtract the offset
    \param Ofs      Offset to subtract in BYTES (not in Pointer element type value)
    \return A pointer of type 'type', pointer at the address of Ptr - Ofs
    */
#define BEHAVIAC_SUB_PTR(type, Ptr, Ofs)            ((type) (((size_t)(Ptr)) - (size_t)(Ofs)))

    // Safe cast from one type to another.  Prevent aliasing.
    // For why this is necessary, read:
    // http://www.cellperformance.com/mike_acton/2006/06/understanding_strict_aliasing.html
    template <typename DST, typename SRC>
    inline DST unaliased_cast(SRC source)
    {
        union UnaliasedConverter
        {
            SRC source;
            DST destination;
        };
        BEHAVIAC_STATIC_ASSERT(sizeof(DST) == sizeof(SRC));
        UnaliasedConverter converter;
        converter.source = source;
        return converter.destination;
    }

    /// Returns the value of the constant PI.
    template <typename T>
    inline T GetPI()
    {
        return T(3.1415926535897932384626433832795);
    }

    /// Returns the value of the constant 2*PI.
    template <typename T>
    inline T GetTwoPI()
    {
        return T(6.2831853071795864769252867665590);
    }

    /// Returns the value of the constant PI/2.
    template <typename T>
    inline T GetHalfPI()
    {
        return T(1.5707963267948966192313216916395);
    }

    /// Returns the precision on the type
    template <typename T>
    inline T GetEpsilon()
    {
        return T(Float32_Epsilon);
    }

    /// behaviac default nan number
    /*! Will only work for floating point number, but provided for all type for convenience.
    */
    template <typename T>
    inline T GetNAN()
    {
        return 0;
    }

    /// behaviac default positive infinite number
    /*! Only useful for floating point number, but provided for all type for convenience.
    */
    template <typename T>
    inline T GetPositiveINF()
    {
        return 0;
    }

    /// behaviac default negative infinite number
    /*! Only useful for floating point number, but provided for all type for convenience.
    */
    template <typename T>
    inline T GetNegativeINF()
    {
        return 0;
    }

    /// behaviac default (positive) infinite number
    /*! Only useful for floating point number, but provided for all type for convenience.
    */
    template <typename T>
    inline T GetINF()
    {
        return GetPositiveINF<T>();
    }

    template <> inline Float32 GetNAN<>()
    {
        const uint32_t F32_NAN = 0x7FFFFFFF;
        // Prevent aliasing problems
        volatile union
        {
            Float32     valF32;
            uint32_t    valU32;
        } aux;
        aux.valU32 = F32_NAN;
        return aux.valF32;
    }

    template <> inline Float32 GetPositiveINF<>()
    {
        const uint32_t F32_INF = 0x7F800000;
        // Prevent aliasing problems
        volatile union
        {
            Float32     valF32;
            uint32_t         valU32;
        } aux;
        aux.valU32 = F32_INF;
        return aux.valF32;
    }

    template <> inline Float32 GetNegativeINF<>()
    {
        const uint32_t F32_NEG_INF = 0xFF800000;
        return unaliased_cast<Float32>(F32_NEG_INF);
    }

    /// Returns TRUE if the value is 0 or lower than the margin of error
    /*! This function is slower than a == 0.0f with a float, but when doing complex calculations,
    a float value will be slightly different due to precision limitation.
    */
    inline bool  IsNullWithEpsilon(Float32 Value, Float32 Epsilon = Float32_Epsilon)
    {
        return fabsf(Value) <= Epsilon;
    }

    /// Returns TRUE if two values are equal or closer than the margin of error
    /*! This function is slower than a == between two types, but when doing complex calculations,
    two float value will be slightly different due to precision limitation.
    */
    inline bool  IsEqualWithEpsilon(Float32 x, Float32 y, Float32 Epsilon = (Float32_Epsilon * 2))
    {
        return fabsf(x - y) <= Epsilon;
    }

    /// Returns TRUE if two values are equal or closer than the margin of error
    /*! This function is slower than a == between two types, but when doing complex calculations,
    two float value will be slightly different due to precision limitation.
    */
    inline bool  IsEqualWithEpsilon(Float64 x, Float64 y, Float64 Epsilon = (Float32_Epsilon * 2))
    {
        return fabs(x - y) <= Epsilon;
    }

    /// Calculates the square root
    /*! The GetSquareRoot function returns the square-root of x. If x is negative, it returns an indefinite, by default
    */
    inline Float32 GetSquareRoot(Float32 x)
    {
        return sqrtf(x);
    }

    /// Calculates the inverse square root
    /*! The GetRsqrt function returns the inverse of the square-root of x. If x is negative, it returns an indefinite, by default
    */
    inline Float32 GetRsqrt(Float32 x)
    {
        return 1.0f / GetSquareRoot(x);
    }

    /// Round to the nearest integer value
    inline Float32 GetRound(Float32 x)
    {
        return (Float32)((int)(x + (x > 0.0f ? 0.5f : -0.5f)));
    }

    ///// Calculates the ceiling of a value
    ///*! The ceil function returns a value representing the smallest integer that is greater than or equal to x.
    //*/
    //inline Float32 GetRoundUp(Float32 x)
    //{
    //    return ceilf(x);
    //}

    /// Calculates the floor of a value
    /*! The floor function returns a floating-point value representing the largest integer that is less than or equal to x.
    */
    inline Float32 GetRoundDown(Float32 x)
    {
        return floorf(x);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    //                      behaviac signed/unsigned type converter templates
    //
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*! Type Converters are typically used for type casting in templates
    *  Example usage:
    *  \code
    *  uint16_t   sampleU16 = 14;
    *  behaviac::int16_t   sampleS16 = 17;
    *
    *  sampleU16 = (behaviac::ConvertToUnsigned<behaviac::int16_t>::Type)sampleS16
    *  \endcode
    *
    *  Where <behaviac::int16_t> is typically replaced by &lt;T&gt; when put in a template
    */

    /// Convert a basic integer type into it's unsigned version.
    /*! If the signed value is negative, unpredictable results will occur
    */
    template <typename TYPE> struct ConvertToUnsigned
    {
        typedef TYPE Type;
    };

    template<> struct ConvertToUnsigned<int8_t>
    {
        typedef uint8_t Type;
    };

    template<> struct ConvertToUnsigned<char>
    {
        typedef uint8_t Type;
    };

    template<> struct ConvertToUnsigned<int16_t>
    {
        typedef uint16_t Type;
    };

    template<> struct ConvertToUnsigned<int32_t>
    {
        typedef uint32_t Type;
    };

    template<> struct ConvertToUnsigned<int64_t>
    {
        typedef uint64_t Type;
    };

    /// Convert a basic integer type into it's signed version.
    template <typename TYPE> struct ConvertToSigned
    {
        typedef TYPE Type;
    };

    template<> struct ConvertToSigned<uint8_t>
    {
        typedef int8_t Type;
    };

    template<> struct ConvertToSigned<uint16_t>
    {
        typedef int16_t Type;
    };

    template<> struct ConvertToSigned<uint32_t>
    {
        typedef int32_t Type;
    };

    template<> struct ConvertToSigned<uint64_t>
    {
        typedef int64_t Type;
    };

    template<typename T>
    inline bool InRange(const T& Value, const T& Min, const T& Max)
    {
        return (Min <= Value) && (Value <= Max);
    }

    template<typename T>
    inline T ClampValue(const T& Value, const T& Min, const T& Max)
    {
        T returnVal = Value;

        if (Value < Min)
        {
            returnVal = Min;

        }
        else if (Value > Max)
        {
            returnVal = Max;
        }

        return returnVal;
    }

    template<typename T>
    inline void SwapValues(T& a, T& b)
    {
        T Temp = a;
        a = b;
        b = Temp;
    }

    /// Return the minimum value between 2
    template<typename _Type>
    _Type Min(_Type a, _Type b)
    {
        return a < b ? a : b;
    }

    /// Return the maximum value between 2
    template <typename _Type>
    _Type Max(_Type a, _Type b)
    {
        return a > b ? a : b;
    }
    /// Return the minimum value between 3
    template <typename _Type>
    inline _Type Min(_Type a, _Type b, _Type c)
    {
        return a < b ? (a < c ? a : c) : (b < c ? b : c);
    }

    /// Return the maximum value between 3
    template <typename _Type>
    inline _Type Max(_Type a, _Type b, _Type c)
    {
        return a > b ? (a > c ? a : c) : (b > c ? b : c);
    }

    /// Returns the number of bits equal to 1 in the value number.
    /*! \param Value The value from which to count the number of bits
    \return Returns The number of bits equal to 1 in Value
    */
    template <typename _T>
    inline unsigned CountBits(_T Value)
    {
        unsigned Count = 0;

        while (Value != 0)
        {
            Value = Value & (Value - 1);
            Count++;
        }

        return Count;
    }

    /// Returns True if the value in parameter is a power of 2 (Value = 2^x)
    /*! \param Value The value to evaluate
    \return Returns True if the value is a power of 2, False otherwise.
    */
    template <typename _T>
    inline bool IsPowerOf2(_T Value)
    {
        // If value is a power of 2, then it has only one bit.
        // reducing the value by one removes that bit and flips all the other lower bits.
        // the any bits are still the same then its not a power of 2.
        return (!Value || !((Value - 1) & Value));
    }

    /// Rounds a value to the upper value
    /*!
    Align MUST be a power of 2
    \param Val      Value to round
    \param Align    Alignment value
    \return         The nearest multiple of alignment value that is greater or equal
    to Val.
    \code
    int valuetoround = 21;
    int roundedvalue = BEHAVIAC_ROUND(valuetoround, 16); //roundedvalue == 32;
    \endcode

    */
#define BEHAVIAC_ROUND(Val, Align)                  ((((size_t)Val) + ((Align)-1)) & ~((Align)-1))

    /// Rounds a value to its lower value
    /*!
    Align MUST be a power of 2
    \param Val      Value to round
    \param Align    Alignment value
    \return         The nearest multiple of alignment value that is lower or equal
    to Val.
    \code
    int valuetoround = 21;
    int roundedvalue = BEHAVIAC_FLOOR(valuetoround, 16); //roundedvalue == 16;
    \endcode
    */
#define BEHAVIAC_FLOOR(Val, Align)                  (((size_t)Val) & ~((Align) - 1))

    /// Verify if a value is aligned
    /*! \param Val   The value on which we want to verify the alignment
    \param Align The Alignment value. Must be a multiple of 2, >= 1 (1 accepted).
    \return true if value is aligned, false otherwise
    */
#define BEHAVIAC_ALIGNED(Val, Align)                ((((size_t)Val) & ((Align) - 1)) == 0)

    // This is a macro used to convert pointer into 32 bits value
    // This is not portable macro on 64 bits machine
    // But at least, we can quickly identify which module is
    // Assuming a 32 bit value as a pointer by searching this macro
#define BEHAVIAC_PTR_TO_ADDR(ptr)   ((behaviac::Address)((size_t)(ptr)))
#define BEHAVIAC_ADDR_TO_PTR(addr)  ((void*)((size_t)(addr)))

    /// Verify if a pointer is aligned.
    /*! \param Ptr      Pointer on which to verify alignment
    \param Align    Alignment value
    \return true if value is aligned, false otherwise
    */
#define BEHAVIAC_PTR_ALIGNED(Ptr, Align)            BEHAVIAC_ALIGNED(BEHAVIAC_PTR_TO_ADDR(Ptr), (Align))

    /// Round a Pointer to its lower address
    /*! \param type     The return type (typically another pointer)
    \param Ptr      The pointer to round
    \param Ofs      The rounding value
    \return A pointer of type 'type', pointer at the address of Ptr rounded to the lowest Align value
    */
#define BEHAVIAC_ALIGN_PTR(type, Ptr, Ofs)          ((type) BEHAVIAC_FLOOR((size_t)(Ptr), Ofs))

    /// Return the difference between two pointers (First - Second)
#define BEHAVIAC_DIFF_PTR(First, Second)            ((size_t) (((size_t)(First)) - (size_t)(Second)))

#if _MSC_VER >= 1400
    template<typename T_> struct alignment_of
    {
        static T_& t;
        enum { value = __alignof(t) };
    };

#define BEHAVIAC_ALIGNOF(T)     behaviac::alignment_of<T>::value
#else
    //template<typename T>
    //struct __fake_struct_align_
    //{
    //	char c;
    //	T& m;
    //};

#define BEHAVIAC_ALIGNOF(T)		__alignof(T)
#endif//#elif _MSC_VER >= 1400

    /// Get the minimum alignment needed by a type
    template <typename T>
    inline uint32_t AlignOf()
    {
        return BEHAVIAC_ALIGNOF(T);
    }

    template <>
    inline uint32_t AlignOf<void>()
    {
        return 1;
    }

    /// Make a word(16,32,64) from 2 sub part
    /*! Returns a word from 2 half-word part
    \code
    uint8_t half_high = 0x12;
    uint8_t half_low = 0x34;
    uint16_t fullword = BEHAVIAC_MAKE16(half_low, half_high); //fullword == 0x1234
    \endcode
    */
#define BEHAVIAC_MAKE16(Low, High)   ((uint16_t) (((uint8_t)  (Low)) | ((uint16_t) ((uint8_t)  (High))) << 8))
#define BEHAVIAC_MAKE32(Low, High)   ((uint32_t) (((uint16_t) (Low)) | ((uint32_t) ((uint16_t) (High))) << 16))
#define BEHAVIAC_MAKE64(Low, High)   ((uint64_t) (((uint32_t) (Low)) | ((uint64_t) ((uint32_t) (High))) << 32))

    /// Get the 2 half-word from one word(16,32,64)
    /*! Return low and high part of a word(16,32,64)
    \code
    uint16_t fullword = 0x1234
    uint8_t half_low = BEHAVIAC_LO8(fullword) //half_low == 0x34
    uint8_t half_high = BEHAVIAC_HI8(fullword) //half_high == 0x12
    \endcode
    */
#define BEHAVIAC_LO8(Operand16Bit)                  ((uint8_t)((size_t)((Operand16Bit) & 0xFF)))
#define BEHAVIAC_HI8(Operand16Bit)                  ((uint8_t)(((size_t)(Operand16Bit) >> 8)  & 0xFF))
#define BEHAVIAC_LO16(Operand32Bit)                 ((uint16_t)((size_t)((Operand32Bit) & 0xFFFF)))
#define BEHAVIAC_HI16(Operand32Bit)                 ((uint16_t)(((size_t)(Operand32Bit) >> 16) & 0xFFFF))
#define BEHAVIAC_LO32(Operand64Bit)                 ((uint32_t) ((Operand64Bit) & 0xFFFFFFFF))
#define BEHAVIAC_HI32(Operand64Bit)                 ((uint32_t) (((uint64_t)(Operand64Bit) >> 32) & 0xFFFFFFFF))

    // DEPRECATED - USE BEHAVIAC_LO32/BEHAVIAC_HI32 INSTEAD
#define BEHAVIAC_LOW32(Operand64Bit)                BEHAVIAC_LO32(Operand64Bit)
#define BEHAVIAC_HIW32(Operand64Bit)                BEHAVIAC_HI32(Operand64Bit)

#define BEHAVIAC_KB(Nb)                     ((Nb)*1024U)            ///< Defines for KiloBytes prefix (2 ^ 10)
#define BEHAVIAC_MB(Nb)                     BEHAVIAC_KB((Nb)*1024U)     ///< Defines for MegaBytes prefix (2 ^ 20)
#define BEHAVIAC_GB(Nb)                     BEHAVIAC_MB((Nb)*1024ULL)   ///< Defines for GigaBytes prefix (2 ^ 30)
#define BEHAVIAC_TB(Nb)                     BEHAVIAC_GB((Nb)*1024ULL)   ///< Defines for TeraBytes prefix (2 ^ 40)
#define BEHAVIAC_PB(Nb)                     BEHAVIAC_TB((Nb)*1024ULL)   ///< Defines for TeraBytes prefix (2 ^ 40)
}//end of ns

/*! @} */
/*! @} */

#endif//BEHAVIAC_BASE_CORE_TYPES_H
