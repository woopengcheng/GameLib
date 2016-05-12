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

#ifndef BEHAVIAC_TYPETRAITS_H
#define BEHAVIAC_TYPETRAITS_H

#if _MSC_VER >= 1400
#define HAS_USER_DESTRUCTOR(T) __has_user_destructor(T)
#define HAS_TRIVIAL_DESTRUCTOR(T) __has_trivial_destructor(T)
#define IS_POD(T) __is_pod(T)
#define IS_ENUM(T) __is_enum(T)
#else
#define HAS_USER_DESTRUCTOR(T) (true)
#define HAS_TRIVIAL_DESTRUCTOR(T) (true)
#define IS_POD(T) (false)
#define IS_ENUM(T) (false)
#endif

namespace behaviac
{
    template<typename T> struct IsAFloat
    {
        enum { Value = false };
    };

    template<> struct IsAFloat<float>
    {
        enum { Value = true };
    };
    template<> struct IsAFloat<double>
    {
        enum { Value = true };
    };
    template<> struct IsAFloat<long double>
    {
        enum { Value = true };
    };

    template<typename T> struct IsAnInteger
    {
        enum { Value = false };
    };

    template<> struct IsAnInteger<unsigned char>
    {
        enum { Value = true };
    };
    template<> struct IsAnInteger<unsigned short>
    {
        enum { Value = true };
    };
    template<> struct IsAnInteger<unsigned int>
    {
        enum { Value = true };
    };
    template<> struct IsAnInteger<unsigned long>
    {
        enum { Value = true };
    };

    template<> struct IsAnInteger<signed char>
    {
        enum { Value = true };
    };
    template<> struct IsAnInteger<signed short>
    {
        enum { Value = true };
    };
    template<> struct IsAnInteger<signed int>
    {
        enum { Value = true };
    };
    template<> struct IsAnInteger<signed long>
    {
        enum { Value = true };
    };

    template<> struct IsAnInteger<bool>
    {
        enum { Value = true };
    };
    template<> struct IsAnInteger<char>
    {
        enum { Value = true };
    };

    // compilers we support define wchar_t as a native type
#if !_MSC_VER || defined(_NATIVE_WCHAR_T_DEFINED)
    template<> struct IsAnInteger<wchar_t>
    {
        enum { Value = true };
    };
#endif

    template<> struct IsAnInteger<unsigned long long>
    {
        enum { Value = true };
    };
    template<> struct IsAnInteger<signed long long>
    {
        enum { Value = true };
    };

    template<typename T> struct IsArithmetic
    {
        enum { Value = IsAnInteger<T>::Value || IsAFloat<T>::Value };
    };

    template<typename T> struct IsPointer
    {
        enum { Value = false };
    };
    template<typename T> struct IsPointer<T*>
    {
        enum { Value = true };
    };
    template<typename T> struct IsPointer<const T*>
    {
        enum { Value = true };
    };
    template<typename T> struct IsPointer<const T* const>
    {
        enum { Value = true };
    };
    template<typename T> struct IsPointer<T* volatile>
    {
        enum { Value = true };
    };
    template<typename T> struct IsPointer<T* const volatile>
    {
        enum { Value = true };
    };

    template<typename T> struct IsVoid
    {
        enum { Value = false };
    };
    template<> struct IsVoid<void>
    {
        enum { Value = true };
    };
    template<> struct IsVoid<void const>
    {
        enum { Value = true };
    };
    template<> struct IsVoid<void volatile>
    {
        enum { Value = true };
    };
    template<> struct IsVoid<void const volatile>
    {
        enum { Value = true };
    };

    template<typename T> struct IsPOD
    {
        enum { Value = IS_POD(T) || IS_ENUM(T) || IsArithmetic<T>::Value || IsPointer<T>::Value };
    };

    template<typename T>
    struct IsFundamental
    {
        enum { Value = IsArithmetic<T>::Value || IsVoid<T>::Value };
    };

    template<typename T> struct HasDestructor
    {
        enum { value = ((!HAS_TRIVIAL_DESTRUCTOR(T) && !IsPOD<T>::Value) || HAS_USER_DESTRUCTOR(T)) };
    };
}

#endif//BEHAVIAC_TYPETRAITS_H
