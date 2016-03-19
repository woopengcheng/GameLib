/*
 * Copyright 2012 10gen Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

/**
 * Include "mongo/platform/cstdint.h" to get the C++11 cstdint types in namespace mongo.
 */ 
#if defined(_MSC_VER)
//#include <cstdint>
#define _MONGO_STDINT_NAMESPACE std
#elif defined(__GNUC__)
#include <stdint.h>
#define _MONGO_STDINT_NAMESPACE
#else
#error "Unsupported compiler family"
#endif

namespace _bson {
//     using _MONGO_STDINT_NAMESPACE::int8_t;
//     using _MONGO_STDINT_NAMESPACE::int16_t;
//     using _MONGO_STDINT_NAMESPACE::int32_t;
//     using _MONGO_STDINT_NAMESPACE::int64_t;
//     using _MONGO_STDINT_NAMESPACE::intptr_t;
// 
//     using _MONGO_STDINT_NAMESPACE::uint8_t;
//     using _MONGO_STDINT_NAMESPACE::uint16_t;
//     using _MONGO_STDINT_NAMESPACE::uint32_t;
//     using _MONGO_STDINT_NAMESPACE::uint64_t;
//     using _MONGO_STDINT_NAMESPACE::uintptr_t;

	typedef char int8_t ;
	typedef short int16_t ;
	typedef int int32_t ;
	typedef long long int64_t;

	typedef unsigned char uint8_t;
	typedef unsigned short uint16_t;
	typedef unsigned int uint32_t;
	typedef unsigned long long uint64_t;

// 	using _MONGO_STDINT_NAMESPACE::int_least8_t; using _MONGO_STDINT_NAMESPACE::int_least16_t;
// 	using _MONGO_STDINT_NAMESPACE::int_least32_t;  using _MONGO_STDINT_NAMESPACE::int_least64_t;
// 	using _MONGO_STDINT_NAMESPACE::uint_least8_t; using _MONGO_STDINT_NAMESPACE::uint_least16_t;
// 	using _MONGO_STDINT_NAMESPACE::uint_least32_t; using _MONGO_STDINT_NAMESPACE::uint_least64_t;
// 
// 	using _MONGO_STDINT_NAMESPACE::int_fast8_t; using _MONGO_STDINT_NAMESPACE::int_fast16_t;
// 	using _MONGO_STDINT_NAMESPACE::int_fast32_t;  using _MONGO_STDINT_NAMESPACE::int_fast64_t;
// 	using _MONGO_STDINT_NAMESPACE::uint_fast8_t; using _MONGO_STDINT_NAMESPACE::uint_fast16_t;
// 	using _MONGO_STDINT_NAMESPACE::uint_fast32_t; using _MONGO_STDINT_NAMESPACE::uint_fast64_t;
// 
// 	using _MONGO_STDINT_NAMESPACE::intmax_t; using _MONGO_STDINT_NAMESPACE::intptr_t;
// 	using _MONGO_STDINT_NAMESPACE::uintmax_t; using _MONGO_STDINT_NAMESPACE::uintptr_

}  // namespace mongo

#undef _MONGO_STDINT_NAMESPACE
