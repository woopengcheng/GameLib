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

#ifndef BEHAVIAC_BASE_BASECLASS_H
#define BEHAVIAC_BASE_BASECLASS_H

namespace behaviac
{
    /// BaseClass is the class from which all TAG objects derivate
    class BEHAVIAC_API BaseClass
    {
    public:
        BaseClass() {}

    private:
    };

    /// All cloneable object must inherit from this class. It allows to duplicate inherited object when we only know it's base class.
    class BEHAVIAC_API Cloneable : public BaseClass
    {
    public:
        virtual ~Cloneable() {}

        virtual Cloneable* Clone() const = 0;
        virtual void Destroy() = 0;
    };

    /// Macro that will assure the copy constructor isn't used
    /*! Example :
    	\code
    	class MyClass
    	{
    	CLASS_COPY_NOT_ALLOWED(MyClass)

    	public:
    	MyClass();
    	};
    	\endcode
    	*/
#define CLASS_COPY_NOT_ALLOWED(ClassName)                   \
private:                                                \
    ClassName (const ClassName &);                      \
    const ClassName & operator=(const ClassName &)
}//end of ns

#endif // #ifndef BEHAVIAC_BASE_BASECLASS_H
