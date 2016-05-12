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

#include "typetraits.h"

//#if BEHAVIAC_COMPILER_64BITS
//	#define BEHAVIAC_DEFAULT_ALIGN	8
//#else
//	#define BEHAVIAC_DEFAULT_ALIGN	4
//#endif
#define BEHAVIAC_DEFAULT_ALIGN	8

///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////
namespace behaviac
{
    namespace Private
    {
        BEHAVIAC_FORCEINLINE void* MemHelperAlloc(behaviac::IMemAllocator* pAllocator, size_t TotalSize, unsigned int alignment, const char* tag, const char* pFile, unsigned int Line)
        {
            BEHAVIAC_UNUSED_VAR(pFile);
            BEHAVIAC_UNUSED_VAR(Line);
            BEHAVIAC_UNUSED_VAR(tag);
            void* ptr = 0;

            if (alignment > BEHAVIAC_DEFAULT_ALIGN)
            {
                ptr = BEHAVIAC_ALLOCATOR_MALLOCALIGNED_WITHBEHAVIAC_FILELINE(pAllocator, TotalSize, alignment, tag, pFile, Line);
            }
            else
            {
                ptr = BEHAVIAC_ALLOCATOR_MALLOC_WITHBEHAVIAC_FILELINE(pAllocator, TotalSize, tag, pFile, Line);
            }

            return ptr;
        }

        BEHAVIAC_FORCEINLINE void MemHelperFree(behaviac::IMemAllocator* pAllocator, void* pObj, int alignment, const char* tag, const char* pFile, unsigned int Line)
        {
            BEHAVIAC_UNUSED_VAR(tag);
            BEHAVIAC_UNUSED_VAR(pFile);
            BEHAVIAC_UNUSED_VAR(Line);

            if (alignment > BEHAVIAC_DEFAULT_ALIGN)
            {
                BEHAVIAC_ALLOCATOR_FREEALIGNED_WITHBEHAVIAC_FILELINE(pAllocator, pObj, alignment, tag, pFile, Line);
            }
            else
            {
                BEHAVIAC_ALLOCATOR_FREE_WITHBEHAVIAC_FILELINE(pAllocator, pObj, tag, pFile, Line);
            }
        }

        // We need a template for the delete, since we want to call the destructor
        template<typename T>
        BEHAVIAC_FORCEINLINE void MemHelperDelete(T* pObj, const char* tag, const char* pFile, unsigned int Line)
        {
            BEHAVIAC_UNUSED_VAR(tag);
            BEHAVIAC_UNUSED_VAR(pFile);
            BEHAVIAC_UNUSED_VAR(Line);

            // pObj can be NULL. We don't want to verify in the macro, since pObj could be a function call...
            if (pObj)
            {
                pObj->~T();

                behaviac::IMemAllocator* pAllocator = behaviac::GetAllocator<T>();
                behaviac::Private::MemHelperFree(pAllocator, pObj, BEHAVIAC_ALIGNOF(T), tag, pFile, Line);
            }
        }

        BEHAVIAC_FORCEINLINE void* MemHelperAllocAlignment(behaviac::IMemAllocator* pAllocator, size_t TotalSize, unsigned int alignment, const char* tag, const char* pFile, unsigned int Line)
        {
            BEHAVIAC_UNUSED_VAR(pFile);
            BEHAVIAC_UNUSED_VAR(Line);
            BEHAVIAC_UNUSED_VAR(tag);

            void* ptr = BEHAVIAC_ALLOCATOR_MALLOCALIGNED_WITHBEHAVIAC_FILELINE(pAllocator, TotalSize, alignment, tag, pFile, Line);

            return ptr;
        }

        BEHAVIAC_FORCEINLINE void MemHelperFreeAlignment(behaviac::IMemAllocator* pAllocator, void* pObj, int alignment, const char* tag, const char* pFile, unsigned int Line)
        {
            BEHAVIAC_UNUSED_VAR(tag);
            BEHAVIAC_UNUSED_VAR(pFile);
            BEHAVIAC_UNUSED_VAR(Line);

            BEHAVIAC_ALLOCATOR_FREEALIGNED_WITHBEHAVIAC_FILELINE(pAllocator, pObj, alignment, tag, pFile, Line);
        }

        // We need a template for the delete, since we want to call the destructor
        template<typename T>
        BEHAVIAC_FORCEINLINE void MemHelperDeleteAlignment(T* pObj, int align, const char* tag, const char* pFile, unsigned int Line)
        {
            BEHAVIAC_UNUSED_VAR(tag);
            BEHAVIAC_UNUSED_VAR(pFile);
            BEHAVIAC_UNUSED_VAR(Line);

            // pObj can be NULL. We don't want to verify in the macro, since pObj could be a function call...
            if (pObj)
            {
                pObj->~T();

                behaviac::IMemAllocator* pAllocator = behaviac::GetAllocator<T>();
                behaviac::Private::MemHelperFreeAlignment(pAllocator, pObj, align, tag, pFile, Line);
            }
        }

        template<typename T>
        struct MemHelperArrayAllocator
        {
            static BEHAVIAC_FORCEINLINE T* AllocArray(behaviac::IMemAllocator* pAllocator, unsigned int Count, const char* tag, const char* pFile, unsigned int Line)
            {
                int alignment = BEHAVIAC_ALIGNOF(T);

                if (alignment < BEHAVIAC_DEFAULT_ALIGN)
                {
                    alignment = BEHAVIAC_DEFAULT_ALIGN;
                }

                BEHAVIAC_ASSERT(alignment >= BEHAVIAC_DEFAULT_ALIGN);
                unsigned int totalSize = BEHAVIAC_ROUND(sizeof(int32_t), alignment) + sizeof(T) * Count;
                void* pData = behaviac::Private::MemHelperAlloc(pAllocator, totalSize, alignment, tag, pFile, Line);
                BEHAVIAC_ASSERT(pData);
                BEHAVIAC_ASSERT(BEHAVIAC_ALIGNED(pData, alignment));
                int32_t* pCount = (int32_t*)pData;
                BEHAVIAC_ASSERT(BEHAVIAC_ALIGNED(pCount, BEHAVIAC_DEFAULT_ALIGN));
                BEHAVIAC_ASSERT(alignment >= BEHAVIAC_DEFAULT_ALIGN);
                T* pArray = (T*)BEHAVIAC_ROUND(pCount + 1, alignment);
                //store the count just before Array's elements
                //pArray might be different from pCount if alignemt > BEHAVIAC_DEFAULT_ALIGN
                *(((int32_t*)pArray) - 1) = (int32_t)Count;
                BEHAVIAC_ASSERT(BEHAVIAC_ALIGNED(pArray, BEHAVIAC_DEFAULT_ALIGN));

                for (int i = 0; i < (int)Count; i++)
                {
                    new(&pArray[i]) T;
                }

                return (T*)pArray;
            }
        };

        template<typename T>
        struct MemHelperArrayAllocator<T*>
        {
            static BEHAVIAC_FORCEINLINE T** AllocArray(behaviac::IMemAllocator* pAllocator, unsigned int Count, const char* tag, const char* pFile, unsigned int Line)
            {
                return (T**)behaviac::Private::MemHelperAlloc(pAllocator, sizeof(T*) * Count, BEHAVIAC_ALIGNOF(T*), tag, pFile, Line);
            }
        };

        template<typename T>
        BEHAVIAC_FORCEINLINE T* MemHelperAllocArray(behaviac::IMemAllocator* pAllocator, unsigned int Count, const char* tag, const char* pFile, unsigned int Line)
        {
            return MemHelperArrayAllocator<T>::AllocArray(pAllocator, Count, tag, pFile, Line);
        }

        template<typename T>
        struct MemHelperArrayDeleter
        {
            static BEHAVIAC_FORCEINLINE void Delete(behaviac::IMemAllocator* pAllocator, T* pArray, const char* tag, const char* pFile, unsigned int Line)
            {
                BEHAVIAC_UNUSED_VAR(tag);
                BEHAVIAC_UNUSED_VAR(pFile);
                BEHAVIAC_UNUSED_VAR(Line);

                if (!pArray)
                {
                    return;
                }

                int alignment = BEHAVIAC_ALIGNOF(T);

                if (alignment < BEHAVIAC_DEFAULT_ALIGN)
                {
                    alignment = BEHAVIAC_DEFAULT_ALIGN;
                }

                BEHAVIAC_ASSERT(alignment >= BEHAVIAC_DEFAULT_ALIGN);
                BEHAVIAC_ASSERT(BEHAVIAC_ALIGNED(pArray, alignment));
                int32_t* pCount = ((int32_t*)pArray) - 1;
                int count = (int)(*pCount);

                for (int i = count - 1; i >= 0; i--)
                {
                    pArray[i].~T();
                }

                void* ptrOriginal = (void*)BEHAVIAC_FLOOR(pCount, alignment);
                behaviac::Private::MemHelperFree(pAllocator, ptrOriginal, alignment, tag, pFile, Line);
            }
        };

        template<typename T>
        struct MemHelperArrayDeleter<T*>
        {
            //We need this flavor since when we have an array of pointers, we don't want to call destructors
            static BEHAVIAC_FORCEINLINE void Delete(behaviac::IMemAllocator* pAllocator, T** pArray, const char* tag, const char* pFilename, unsigned int LineNo)
            {
                BEHAVIAC_UNUSED_VAR(tag);
                BEHAVIAC_UNUSED_VAR(pFilename);
                BEHAVIAC_UNUSED_VAR(LineNo);

                behaviac::Private::MemHelperFree(pAllocator, pArray, BEHAVIAC_ALIGNOF(T*), tag, pFilename, LineNo);
            }
        };

        template<typename T>
        BEHAVIAC_FORCEINLINE void MemHelperDeleteArray(T* pArray, const char* tag, const char* pFilename, unsigned int LineNo)
        {
            behaviac::IMemAllocator* pAllocator = behaviac::GetAllocator<T>();
            MemHelperArrayDeleter<T>::Delete(pAllocator, pArray, tag, pFilename, LineNo);
        }

        template<class T>
        struct has_destructor
        {
            enum { value = (HasDestructor<T>::value) };
        };

        template<class T, bool hasDtor>
        struct MemHelperArrayDeleterSystem
        {
            static BEHAVIAC_FORCEINLINE void Delete(T* pObj, size_t align, const char* tag, const char* pFile, unsigned int Line)
            {
                BEHAVIAC_UNUSED_VAR(tag);
                BEHAVIAC_UNUSED_VAR(pFile);
                BEHAVIAC_UNUSED_VAR(Line);

                // pObj can be NULL. We don't want to verify in the macro, since pObj could be a function call...
                if (pObj)
                {
                    behaviac::IMemAllocator* pAllocator = behaviac::GetAllocator<T>();
                    behaviac::Private::MemHelperFreeAlignment(pAllocator, pObj, align, tag, pFile, Line);
                }
            }
        };

        template<class T>
        struct MemHelperArrayDeleterSystem<T, true>
        {
            static BEHAVIAC_FORCEINLINE void Delete(T* pObj, size_t align, const char* tag, const char* pFile, unsigned int Line)
            {
                BEHAVIAC_UNUSED_VAR(tag);
                BEHAVIAC_UNUSED_VAR(pFile);
                BEHAVIAC_UNUSED_VAR(Line);

                // pObj can be NULL. We don't want to verify in the macro, since pObj could be a function call...
                if (pObj)
                {
                    //TODO: this is only working for msvc?
                    void* ptr = (((int*)pObj) - 1);
                    int* pCount = (int*)BEHAVIAC_FLOOR(ptr, align);
                    int count = *pCount;

                    for (int i = 0; i < count; ++i, ++pObj)
                    {
                        pObj->~T();
                    }

                    behaviac::IMemAllocator* pAllocator = behaviac::GetAllocator<T>();
                    behaviac::Private::MemHelperFreeAlignment(pAllocator, pCount, align, tag, pFile, Line);
                }
            }
        };
    } // namespace Private

    struct STagOperatorNewType
    {
        int m_dummyMember;
        static BEHAVIAC_API const STagOperatorNewType& GetInstance();
    };

    struct STagOperatorNewArrayType
    {
        int m_dummyMember;

        static BEHAVIAC_API const STagOperatorNewArrayType& GetInstance();
    };

    template <typename T>
    inline void operator ^(const STagOperatorNewType& /*type*/, T* object)
    {
        int align = BEHAVIAC_ALIGNOF(T);

        if (align < BEHAVIAC_DEFAULT_ALIGN)
        {
            align = BEHAVIAC_DEFAULT_ALIGN;
        }

        BEHAVIAC_ASSERT(align == BEHAVIAC_DEFAULT_ALIGN, "please include BEHAVIAC_DECLARE_MEMORY_OPERATORS in your type or use BEHAVIAC_G_*!");

        behaviac::Private::MemHelperDeleteAlignment(object, align, "behaviac", __FILE__, __LINE__);
    }

    template <typename T>
    inline void operator ^(const STagOperatorNewArrayType& /*type*/, T* object)
    {
        if (object)
        {
            //MemHelperArrayDeleterSystem is only working for msvc
            BEHAVIAC_ASSERT(0, "please include BEHAVIAC_DECLARE_MEMORY_OPERATORS in your type or use BEHAVIAC_G_*!");

            behaviac::Private::MemHelperArrayDeleterSystem<T, behaviac::Private::has_destructor<T>::value>::Delete(object, BEHAVIAC_ALIGNOF(T), "behaviac", __FILE__, __LINE__);
        }
    }
}//namespace behaviac

BEHAVIAC_FORCEINLINE void* operator new(size_t size, const behaviac::STagOperatorNewType& /*type*/, const char* tag, const char* file, int line)
{
    return behaviac::Private::MemHelperAllocAlignment(&behaviac::GetMemoryAllocator(), size, BEHAVIAC_DEFAULT_ALIGN, tag, file, line);
}
BEHAVIAC_FORCEINLINE void operator delete(void* ptr, const behaviac::STagOperatorNewType& /*type*/, const char* tag, const char* file, int line)
{
    BEHAVIAC_UNUSED_VAR(file);
    BEHAVIAC_UNUSED_VAR(tag);
    BEHAVIAC_UNUSED_VAR(line);
    behaviac::Private::MemHelperFreeAlignment(&behaviac::GetMemoryAllocator(), ptr, BEHAVIAC_DEFAULT_ALIGN, tag, file, line);
}
BEHAVIAC_FORCEINLINE void* operator new[](size_t size, const behaviac::STagOperatorNewArrayType& /*type*/, const char* tag, const char* file, int line)
{
    BEHAVIAC_ASSERT(0, "please include BEHAVIAC_DECLARE_MEMORY_OPERATORS in your type or use BEHAVIAC_G_*!");
    return behaviac::Private::MemHelperAllocAlignment(&behaviac::GetMemoryAllocator(), size, BEHAVIAC_DEFAULT_ALIGN, tag, file, line);
}
BEHAVIAC_FORCEINLINE void operator delete[](void* ptr, const behaviac::STagOperatorNewArrayType& /*type*/, const char* tag, const char* file, int line)
{
    BEHAVIAC_UNUSED_VAR(tag);
    BEHAVIAC_UNUSED_VAR(file);
    BEHAVIAC_UNUSED_VAR(line);
    BEHAVIAC_ASSERT(0, "please include BEHAVIAC_DECLARE_MEMORY_OPERATORS in your type or use BEHAVIAC_G_*!");
    behaviac::Private::MemHelperFreeAlignment(&behaviac::GetMemoryAllocator(), ptr, BEHAVIAC_DEFAULT_ALIGN, tag, file, line);
}

#define BEHAVIAC_DECLARE_MEMORY_OPERATORS_COMMON_(CLASS, align) \
    public: \
    friend BEHAVIAC_FORCEINLINE void operator ^(const behaviac::STagOperatorNewType& type, CLASS * object) \
    { \
        if (object) object->tag_special_delete(type); \
    } \
    friend BEHAVIAC_FORCEINLINE void operator ^(const behaviac::STagOperatorNewArrayType& type, CLASS * object) \
    { \
        if (object) object->tag_special_delete_array(type); \
    }\
    BEHAVIAC_FORCEINLINE void * operator new(size_t size, const behaviac::STagOperatorNewType& /*type*/, const char * tag, const char * file, int line) \
    { \
        return behaviac::Private::MemHelperAllocAlignment(behaviac::GetAllocator<CLASS>(), size, align, tag, file, line); \
    } \
    BEHAVIAC_FORCEINLINE void* operator new[](size_t size, const behaviac::STagOperatorNewArrayType& /*type*/, const char * tag, const char * file, int line) \
    { \
        return behaviac::Private::MemHelperAllocAlignment(behaviac::GetAllocator<CLASS>(), size, align, tag, file, line); \
    } \
    BEHAVIAC_FORCEINLINE void operator delete(void* ptr) \
    { \
        behaviac::Private::MemHelperFreeAlignment(behaviac::GetAllocator<CLASS>(), ptr, align, 0, __FILE__, __LINE__); \
    } \
    BEHAVIAC_FORCEINLINE void operator delete[](void* ptr) \
    { \
        behaviac::Private::MemHelperFreeAlignment(behaviac::GetAllocator<CLASS>(), ptr, align, 0, __FILE__, __LINE__); \
    }\
    BEHAVIAC_FORCEINLINE void operator delete(void * ptr, const behaviac::STagOperatorNewType& /*type*/, const char * tag, const char * file, int line) \
    { \
        BEHAVIAC_UNUSED_VAR(tag);\
        BEHAVIAC_UNUSED_VAR(file);\
        BEHAVIAC_UNUSED_VAR(line);\
        behaviac::Private::MemHelperFreeAlignment(behaviac::GetAllocator<CLASS>(), ptr, align, tag, file, line); \
    } \
    BEHAVIAC_FORCEINLINE void operator delete[](void * ptr, const behaviac::STagOperatorNewArrayType& /*type*/, const char * tag, const char * file, int line) \
    { \
        BEHAVIAC_UNUSED_VAR(tag);\
        BEHAVIAC_UNUSED_VAR(file);\
        BEHAVIAC_UNUSED_VAR(line);\
        behaviac::Private::MemHelperFreeAlignment(behaviac::GetAllocator<CLASS>(), ptr, align, tag, file, line); \
    } \
    private:\
    BEHAVIAC_FORCEINLINE void tag_special_delete_array(const behaviac::STagOperatorNewArrayType& /*type*/) const \
    { \
        /*operator delete[]((void*)this, type, __FILE__, __LINE__);*/\
        delete[] this;\
    } \
    public :

#define BEHAVIAC_DECLARE_MEMORY_OPERATORS_(CLASS, align) \
    BEHAVIAC_DECLARE_MEMORY_OPERATORS_COMMON_(CLASS, align) \
	BEHAVIAC_FORCEINLINE void tag_special_delete(const behaviac::STagOperatorNewType& /*type*/) const \
    { \
        /*operator delete((void*)this, type, __FILE__, __LINE__);*/\
        delete this;\
    } \
	public :

#define BEHAVIAC_DECLARE_MEMORY_OPERATORS_AGENT_(CLASS, align) \
	BEHAVIAC_DECLARE_MEMORY_OPERATORS_COMMON_(CLASS, align) \
	BEHAVIAC_FORCEINLINE void tag_special_delete(const behaviac::STagOperatorNewType& /*type*/) \
    { \
        /*operator delete((void*)this, type, __FILE__, __LINE__);*/\
        destroy_();\
    } \
	public :
