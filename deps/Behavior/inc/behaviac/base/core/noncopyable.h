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

#ifndef BEHAVIAC_BASE_NONCOPYABLE_H
#define BEHAVIAC_BASE_NONCOPYABLE_H

#include "behaviac/base/core/config.h"

namespace behaviac
{
    /**
     * Derive from this to stop a class from being able to be
     * copied, either via copy constructor or assignment.
     */
    class BEHAVIAC_API NonCopyable
    {
    protected:
        NonCopyable(void) { }
        ~NonCopyable(void) { }
    private:
        NonCopyable(const NonCopyable&);
        NonCopyable& operator=(const NonCopyable&);
    };
}//namespace behaviac

#endif // BEHAVIAC_BASE_NONCOPYABLE_H
