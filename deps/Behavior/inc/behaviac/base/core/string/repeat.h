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

#ifndef BEHAVIAC_REPEAT_H
#define BEHAVIAC_REPEAT_H

#include "behaviac/base/core/string/join.h"

#define REPEATS0( token )
#define REPEATS1( token ) JOIN(token,0)
#define REPEATS2( token ) REPEATS1( token ) , JOIN(token,1)
#define REPEATS3( token ) REPEATS2( token ) , JOIN(token,2)
#define REPEATS4( token ) REPEATS3( token ) , JOIN(token,3)
#define REPEATS5( token ) REPEATS4( token ) , JOIN(token,4)
#define REPEATS6( token ) REPEATS5( token ) , JOIN(token,5)
#define REPEATS7( token ) REPEATS6( token ) , JOIN(token,6)
#define REPEATS8( token ) REPEATS7( token ) , JOIN(token,7)
#define REPEATS9( token ) REPEATS8( token ) , JOIN(token,8)
#define REPEATS10( token ) REPEATS9( token ) , JOIN(token,9)
#define REPEATS11( token ) REPEATS10( token ) , JOIN(token,10)
#define REPEATS12( token ) REPEATS11( token ) , JOIN(token,11)
#define REPEATS13( token ) REPEATS12( token ) , JOIN(token,12)
#define REPEATS14( token ) REPEATS13( token ) , JOIN(token,13)
#define REPEATS15( token ) REPEATS14( token ) , JOIN(token,14)
#define REPEATS( token, num ) JOIN(REPEATS,num)( token )

#define REPEATM0( mac, separator )
#define REPEATM1( mac, separator ) mac(0)
#define REPEATM2( mac, separator ) REPEATM1( mac, separator ) separator() mac(1)
#define REPEATM3( mac, separator ) REPEATM2( mac, separator ) separator() mac(2)
#define REPEATM4( mac, separator ) REPEATM3( mac, separator ) separator() mac(3)
#define REPEATM5( mac, separator ) REPEATM4( mac, separator ) separator() mac(4)
#define REPEATM6( mac, separator ) REPEATM5( mac, separator ) separator() mac(5)
#define REPEATM7( mac, separator ) REPEATM6( mac, separator ) separator() mac(6)
#define REPEATM8( mac, separator ) REPEATM7( mac, separator ) separator() mac(7)
#define REPEATM9( mac, separator ) REPEATM8( mac, separator ) separator() mac(8)
#define REPEATM10( mac, separator ) REPEATM9( mac, separator ) separator() mac(9)
#define REPEATM11( mac, separator ) REPEATM10( mac, separator ) separator() mac(10)
#define REPEATM12( mac, separator ) REPEATM11( mac, separator ) separator() mac(11)
#define REPEATM13( mac, separator ) REPEATM12( mac, separator ) separator() mac(12)
#define REPEATM14( mac, separator ) REPEATM13( mac, separator ) separator() mac(13)
#define REPEATM15( mac, separator ) REPEATM14( mac, separator ) separator() mac(14)
#define REPEATM( mac, separator, num ) JOIN(REPEATM,num)( mac, separator )

#define REPEAT_SEPARATOR_COMMA() ,
#define REPEAT_SEPARATOR_SPACE()
#define REPEAT_SEPARATOR_SEMICOLON() ;

#define REPEAT( mac, num ) REPEATM( mac, REPEAT_SEPARATOR_COMMA, num )

#endif // BEHAVIAC_REPEAT_H
